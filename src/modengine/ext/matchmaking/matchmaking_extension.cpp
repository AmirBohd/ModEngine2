#pragma once

#include "proto/Frpg2RequestMessage.pb.h"
#include "matchmaking_extension.h"

#include <map>
#include <cerrno>
#include <spdlog/fmt/bin_to_hex.h>

namespace modengine::ext {

#include "matchmaking_protobuf_vtables.h"

std::atomic_long counter;
std::shared_ptr<Hook<fpProtobufDeserialize>> hooked_ProtobufDeserialize;
std::shared_ptr<Hook<fpProtobufSerialize>> hooked_ProtobufSerialize;

struct MessageHandler {
    MessageHandler(std::string kind, unsigned char* buffer, size_t buffer_size)
        : m_kind(kind)
        , m_buffer(buffer)
        , m_buffer_size(buffer_size)
    {
    }
    unsigned char* m_buffer;
    size_t m_buffer_size;

    std::string m_kind;
    std::optional<std::string> m_result;

    template <typename T>
    void handle(std::function<void(T*)> callback)
    {
        static T default_msg = T::default_instance();

        if (m_result.has_value() || m_kind != default_msg.GetTypeName()) {
            return;
        }

        auto message = T();
        message.ParseFromArray(m_buffer, (int)m_buffer_size);
        callback(&message);
        m_result = message.SerializeAsString();
    }
};

bool protobuf_deserialize(void* msg, unsigned char* buffer, size_t size)
{
    using namespace Frpg2RequestMessage;

    auto vtable_addr = *(uintptr_t*)msg;
    auto kind = vtable_map[vtable_addr];
    auto handler = MessageHandler(kind, buffer, size);
   
    handler.handle<RequestGetAnnounceMessageListResponse>([&](auto msg) {
        msg->mutable_changes()->clear_items();
    });

    handler.handle<RequestGetSignListResponse>([&](auto msg) {
        auto signData = msg->mutable_getsignresult()->mutable_signdata();

        auto newEnd = std::remove_if(signData->begin(), signData->end(), [](const SignData& signDataItr){
            debug((std::string("Total get soul: ") + std::to_string(signDataItr.matchingparameter().totalgetsoul())).c_str());
            return signDataItr.matchingparameter().totalgetsoul() != 684534681;
        });

        signData->erase(newEnd, signData->end());
    });
    
    auto result = handler.m_result;

    if (result.has_value()) {
        return hooked_ProtobufDeserialize->original(msg, (unsigned char*)result->c_str(), result->length());
    }

    return hooked_ProtobufDeserialize->original(msg, buffer, (int)size);
}

uintptr_t protobuf_serialize(void* msg, unsigned char* buffer)
{    
    using namespace Frpg2RequestMessage;
    auto vtable_addr = *(uintptr_t*)msg;
    auto kind = vtable_map[vtable_addr];

    size_t(*zerocopy_get_cached_size)(...);
    *(uintptr_t*)&zerocopy_get_cached_size = *(uintptr_t*)((char*)vtable_addr + 0x58);
    size_t size = zerocopy_get_cached_size(msg);

    uintptr_t bufferEnd = hooked_ProtobufSerialize->original(msg, buffer);

    auto handler = MessageHandler(kind, buffer, size);
    
    if (kind == "Frpg2RequestMessage.RequestGetSignList") {
        debug("original length {}, original data {}", size, spdlog::to_hex(buffer, buffer + size));
    }

    handler.handle<RequestGetSignList>([&](auto msg) {
        msg->mutable_matchingparameter()->set_totalgetsoul(684534681);
    });


    auto result = handler.m_result;
    
    if (result.has_value()) {
        debug("new length {}, new data {}", result->length(), spdlog::to_hex(result->c_str(), result->c_str() + result->length()));
        memcpy(buffer, result->c_str(), result->length());
        return (uintptr_t)(buffer + result->length());
    }

    return bufferEnd;
}

void MatchmakingExtension::on_attach()
{
    hooked_ProtobufDeserialize = register_hook(DS3, 0x141b5ea10, protobuf_deserialize);
    hooked_ProtobufSerialize = register_hook(DS3, 0x141b5ebf0, protobuf_serialize);
}

void MatchmakingExtension::on_detach()
{
}

}