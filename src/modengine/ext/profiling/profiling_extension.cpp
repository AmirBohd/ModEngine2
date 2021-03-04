#include "optick.h"
#include "modengine/ext/profiling/profiling_extension.h"
#include "modengine/ext/profiling/main_loop.h"
#include "modengine/ext/profiling/thread_hooks.h"

namespace modengine::ext {
static std::unordered_map<const char*, void*> protobufDeserializeVTable = {
     {"FdpLogMessage::Vector", (void*) 0x140c653e0},
     {"FdpLogMessage::LevelUpLog", (void*) 0x140c629b0},
     {"FdpLogMessage::UseMagicLog", (void*) 0x140c65010},
     {"FdpLogMessage::UseMagicLog_Use_magic_info_list", (void*) 0x140c652a0},
     {"FdpLogMessage::ActGestureLog", (void*) 0x140c60a20},
     {"FdpLogMessage::ActGestureLog_Use_gesture_info_list", (void*) 0x140c60cb0},
     {"FdpLogMessage::BrokenEquipmentLog", (void*) 0x140c60df0},
     {"FdpLogMessage::UseItemLog", (void*) 0x140c64bd0},
     {"FdpLogMessage::UseItemLog_Use_item_info_list", (void*) 0x140c64e60},
     {"FdpLogMessage::PurchaseItemLog", (void*) 0x140c62e60},
     {"FdpLogMessage::PurchaseItemLog_Purchase_item_info_list", (void*) 0x140c63040},
     {"FdpLogMessage::GetItemLog", (void*) 0x140c61ed0},
     {"FdpLogMessage::GetItemLog_Get_item_info_list", (void*) 0x140c620b0},
     {"FdpLogMessage::DropItemLog", (void*) 0x140c61540},
     {"FdpLogMessage::DropItemLog_Throw_away_item_list", (void*) 0x140c61720},
     {"FdpLogMessage::LeaveItemLog", (void*) 0x140c62580},
     {"FdpLogMessage::LeaveItemLog_Set_item_info_list", (void*) 0x140c62760},
     {"FdpLogMessage::SaleItemLog", (void*) 0x140c63d60},
     {"FdpLogMessage::SaleItemLog_Sale_item_info_list", (void*) 0x140c63f40},
     {"FdpLogMessage::AccessWarehouseLog", (void*) 0x140c60690},
     {"FdpLogMessage::AccessWarehouseLog_Access_warehouse_info_list", (void*) 0x140c60860},
     {"FdpLogMessage::StrengthenWeaponLog", (void*) 0x140c64190},
     {"FdpLogMessage::StrengthenWeaponLog_Strengthen_weapon_info_list", (void*) 0x140c64360},
     {"FdpLogMessage::RepairEquipmentLog", (void*) 0x140c63a50},
     {"FdpLogMessage::RepairEquipmentLog_Repair_equipment_info_list", (void*) 0x140c63c20},
     {"FdpLogMessage::EquipMagicLog", (void*) 0x140c61970},
     {"FdpLogMessage::ChaneEstusFlaskNumLog", (void*) 0x140c60fe0},
     {"FdpLogMessage::GlobalEventLog", (void*) 0x140c62480},
     {"FdpLogMessage::CreateBloodMessageLog", (void*) 0x140c61190},
     {"FdpLogMessage::EvaluateBloodMessageLog", (void*) 0x140c619e0},
     {"FdpLogMessage::VisitResultLog", (void*) 0x140c65570},
     {"FdpLogMessage::QuickMatchResultLog", (void*) 0x140c63650},
     {"FdpLogMessage::QuickMatchEndLog", (void*) 0x140c63330},
     {"FdpLogMessage::ConvertSaveDataLog", (void*) 0x140c61050},
     {"FdpLogMessage::SystemOptionLog", (void*) 0x140c644a0},
     {"FdpLogMessage::SystemOptionLog_Game_option_setting", (void*) 0x140c64600},
     {"FdpLogMessage::SystemOptionLog_Network_setting", (void*) 0x140c648b0},
     {"Frpg2RequestMessage::ServerPing", (void*) 0x14226bf20},
     {"Frpg2RequestMessage::RequestMeasureUploadBandwidth", (void*) 0x142263a40},
     {"Frpg2RequestMessage::RequestMeasureUploadBandwidthResponse", (void*) 0x142263bc0},
     {"Frpg2RequestMessage::RequestMeasureDownloadBandwidth", (void*) 0x142263780},
     {"Frpg2RequestMessage::RequestMeasureDownloadBandwidthResponse", (void*) 0x1422638c0},
     {"Frpg2RequestMessage::RequestBenchmarkThroughput", (void*) 0x14225cb30},
     {"Frpg2RequestMessage::RequestBenchmarkThroughputResponse", (void*) 0x14225cd10},
     {"Frpg2RequestMessage::RequestGetAnnounceMessageList", (void*) 0x14225e780},
     {"Frpg2RequestMessage::RequestGetAnnounceMessageListResponse", (void*) 0x14225e920},
     {"Frpg2RequestMessage::RequestGetOnlineShopItemList", (void*) 0x142260bd0},
     {"Frpg2RequestMessage::RequestGetOnlineShopItemListResponse", (void*) 0x142260cc0},
     {"Frpg2RequestMessage::RequestCreateBloodstain", (void*) 0x14225d860},
     {"Frpg2RequestMessage::DomainLimitData", (void*) 0x142254bd0},
     {"Frpg2RequestMessage::RequestGetBloodstainList", (void*) 0x14225f060},
     {"Frpg2RequestMessage::RequestGetBloodstainListResponse", (void*) 0x14225f250},
     {"Frpg2RequestMessage::RequestGetDeadingGhost", (void*) 0x14225ff50},
     {"Frpg2RequestMessage::RequestGetDeadingGhostResponse", (void*) 0x142260090},
     {"Frpg2RequestMessage::RequestCreateBloodMessage", (void*) 0x14225d5a0},
     {"Frpg2RequestMessage::RequestCreateBloodMessageResponse", (void*) 0x14225d780},
     {"Frpg2RequestMessage::RequestReCreateBloodMessageList", (void*) 0x1422668f0},
     {"Frpg2RequestMessage::RequestReCreateBloodMessageList_Blood_message_info_list", (void*) 0x142266b60},
     {"Frpg2RequestMessage::RequestReCreateBloodMessageListResponse", (void*) 0x142266a60},
     {"Frpg2RequestMessage::RequestRemoveBloodMessage", (void*) 0x1422683f0},
     {"Frpg2RequestMessage::LocatedBloodMessage", (void*) 0x142256010},
     {"Frpg2RequestMessage::RequestReentryBloodMessage", (void*) 0x142266db0},
     {"Frpg2RequestMessage::RequestReentryBloodMessageResponse", (void*) 0x142266fc0},
     {"Frpg2RequestMessage::BloodMessageDomainLimitData", (void*) 0x1422537d0},
     {"Frpg2RequestMessage::RequestGetBloodMessageList", (void*) 0x14225ed70},
     {"Frpg2RequestMessage::RequestGetBloodMessageListResponse", (void*) 0x14225ef60},
     {"Frpg2RequestMessage::RequestEvaluateBloodMessage", (void*) 0x14225e570},
     {"Frpg2RequestMessage::RequestGetBloodMessageEvaluation", (void*) 0x14225ea30},
     {"Frpg2RequestMessage::RequestGetBloodMessageEvaluationResponse", (void*) 0x14225ebd0},
     {"Frpg2RequestMessage::RequestCreateGhostData", (void*) 0x14225da60},
     {"Frpg2RequestMessage::RequestGetGhostDataList", (void*) 0x142260270},
     {"Frpg2RequestMessage::RequestGetGhostDataListResponse", (void*) 0x142260460},
     {"Frpg2PlayerData::BonfireInfo", (void*) 0x142292800},
     {"Frpg2PlayerData::DefenseInfo", (void*) 0x142293050},
     {"Frpg2PlayerData::CutRateInfo", (void*) 0x142292950},
     {"Frpg2PlayerData::ResistanceInfo", (void*) 0x1422973a0},
     {"Frpg2PlayerData::Equipment", (void*) 0x142293420},
     {"Frpg2PlayerData::PlayerLocation", (void*) 0x1422950e0},
     {"Frpg2PlayerData::LogInfo", (void*) 0x142294b20},
     {"Frpg2PlayerData::PlayData", (void*) 0x142294d10},
     {"Frpg2PlayerData::StatsInfo", (void*) 0x1422975b0},
     {"Frpg2PlayerData::PlayerStatus", (void*) 0x142295220},
     {"Frpg2PlayerData::AllStatus", (void*) 0x142292520},
     {"Frpg2RequestMessage::RequestUpdatePlayerCharacter", (void*) 0x14226a8c0},
     {"Frpg2RequestMessage::RequestUpdatePlayerStatus", (void*) 0x14226aab0},
     {"Frpg2RequestMessage::MatchingParameter", (void*) 0x142256be0},
     {"Frpg2RequestMessage::RequestGetPlayerCharacter", (void*) 0x142261340},
     {"Frpg2RequestMessage::RequestGetPlayerCharacterResponse", (void*) 0x1422617e0},
     {"Frpg2RequestMessage::RequestGetLoginPlayerCharacter", (void*) 0x142260620},
     {"Frpg2RequestMessage::RequestGetLoginPlayerCharacterResponse", (void*) 0x142260700},
     {"Frpg2RequestMessage::PlayerCharacterID", (void*) 0x142257d90},
     {"Frpg2RequestMessage::RequestGetPlayerCharacterList", (void*) 0x142261480},
     {"Frpg2RequestMessage::RequestGetPlayerCharacterListResponse", (void*) 0x142261620},
     {"Frpg2RequestMessage::RequestUpdateLoginPlayerCharacter", (void*) 0x14226a5e0},
     {"Frpg2RequestMessage::RequestUpdateLoginPlayerCharacterResponse", (void*) 0x14226a740},
     {"Frpg2RequestMessage::RequestRegisterRankingData", (void*) 0x1422675a0},
     {"Frpg2RequestMessage::RequestGetRankingData", (void*) 0x142261c60},
     {"Frpg2RequestMessage::RequestGetRankingDataResponse", (void*) 0x142261e30},
     {"Frpg2RequestMessage::RequestGetCharacterRankingData", (void*) 0x14225f940},
     {"Frpg2RequestMessage::RequestGetCharacterRankingDataResponse", (void*) 0x14225fab0},
     {"Frpg2RequestMessage::RequestCountRankingData", (void*) 0x14225d3b0},
     {"Frpg2RequestMessage::RequestCountRankingDataResponse", (void*) 0x14225d4c0},
     {"Frpg2RequestMessage::RequestGetRightMatchingAreaResponse", (void*) 0x1422622a0},
     {"Frpg2RequestMessage::RequestSendMessageToPlayers", (void*) 0x142269170},
     {"Frpg2RequestMessage::RequestGetBreakInTargetList", (void*) 0x14225f410},
     {"Frpg2RequestMessage::RequestGetBreakInTargetListResponse", (void*) 0x14225f6c0},
     {"Frpg2RequestMessage::RequestBreakInTarget", (void*) 0x14225ce90},
     {"Frpg2RequestMessage::PushRequestBreakInTarget", (void*) 0x142258b00},
     {"Frpg2RequestMessage::PushRequestAllowBreakInTarget", (void*) 0x1422588b0},
     {"Frpg2RequestMessage::RequestRejectBreakInTarget", (void*) 0x142267860},
     {"Frpg2RequestMessage::PushRequestRejectBreakInTarget", (void*) 0x142259580},
     {"Frpg2PlayerData::Vector", (void*) 0x1422979b0},
     {"Frpg2RequestMessage::SignInfo", (void*) 0x14226c790},
     {"Frpg2RequestMessage::RequestNotifyRingBell", (void*) 0x142265c50},
     {"Frpg2RequestMessage::RequestNotifyDisconnectSession", (void*) 0x1422646c0},
     {"Frpg2RequestMessage::KillerInfo", (void*) 0x142255de0},
     {"Frpg2RequestMessage::KillEnemyInfo", (void*) 0x142255c90},
     {"Frpg2RequestMessage::PartyMemberInfo", (void*) 0x142257820},
     {"Frpg2RequestMessage::RequestNotifyRegisterCharacter", (void*) 0x1422657a0},
     {"Frpg2RequestMessage::RequestNotifyDie", (void*) 0x142264330},
     {"Frpg2RequestMessage::RequestNotifyKillEnemy", (void*) 0x142264f30},
     {"Frpg2RequestMessage::RequestNotifyKillBoss", (void*) 0x142264bb0},
     {"Frpg2RequestMessage::RequestNotifyJoinMultiplay", (void*) 0x142264810},
     {"Frpg2RequestMessage::RequestNotifyLeaveMultiplay", (void*) 0x1422651c0},
     {"Frpg2RequestMessage::RequestNotifyCreateSignResult", (void*) 0x142263f80},
     {"Frpg2RequestMessage::RequestNotifySummonSignResult", (void*) 0x142265e40},
     {"Frpg2RequestMessage::RequestNotifyBreakInResult", (void*) 0x142263ca0},
     {"Frpg2RequestMessage::RequestNotifyProtoBufLog", (void*) 0x1422655d0},
     {"Frpg2RequestMessage::RequestGetVisitorList", (void*) 0x142262c50},
     {"Frpg2RequestMessage::RequestGetVisitorListResponse", (void*) 0x142262f70},
     {"Frpg2RequestMessage::PushRequestRemoveVisitor", (void*) 0x14225a0c0},
     {"Frpg2RequestMessage::RequestVisit", (void*) 0x14226b020},
     {"Frpg2RequestMessage::PushRequestVisit", (void*) 0x14225a490},
     {"Frpg2RequestMessage::RequestRejectVisit", (void*) 0x142268110},
     {"Frpg2RequestMessage::PushRequestRejectVisit", (void*) 0x142259a90},
     {"Frpg2RequestMessage::SignDomainGetInfo", (void*) 0x14226c350},
     {"Frpg2RequestMessage::SignGetFlags", (void*) 0x14226c5b0},
     {"Frpg2RequestMessage::LocatedRitualMark", (void*) 0x142256150},
     {"Frpg2RequestMessage::RitualMarkDomainLimit", (void*) 0x14226bd80},
     {"Frpg2RequestMessage::QuickMatchRank", (void*) 0x14225ab10},
     {"Frpg2RequestMessage::RequestSearchQuickMatch", (void*) 0x142268ad0},
     {"Frpg2RequestMessage::RequestSearchQuickMatch_Map_id_list", (void*) 0x142268eb0},
     {"Frpg2RequestMessage::RequestSearchQuickMatchResponse", (void*) 0x142268df0},
     {"Frpg2RequestMessage::RequestRegisterQuickMatch", (void*) 0x1422672d0},
     {"Frpg2RequestMessage::RequestUnregisterQuickMatch", (void*) 0x14226a340},
     {"Frpg2RequestMessage::RequestUpdateQuickMatch", (void*) 0x14226abe0},
     {"Frpg2RequestMessage::RequestJoinQuickMatch", (void*) 0x142263370},
     {"Frpg2RequestMessage::RequestAcceptQuickMatch", (void*) 0x14225c8d0},
     {"Frpg2RequestMessage::RequestRejectQuickMatch", (void*) 0x142267b40},
     {"Frpg2RequestMessage::PushRequestJoinQuickMatch", (void*) 0x1422591b0},
     {"Frpg2RequestMessage::PushRequestAcceptQuickMatch", (void*) 0x1422586f0},
     {"Frpg2RequestMessage::PushRequestRejectQuickMatch", (void*) 0x142259830},
     {"Frpg2RequestMessage::RequestCreateSign", (void*) 0x14225e040},
     {"Frpg2RequestMessage::RequestCreateSignResponse", (void*) 0x14225e320},
     {"Frpg2RequestMessage::RequestUpdateSign", (void*) 0x14226ae10},
     {"Frpg2RequestMessage::RequestRemoveSign", (void*) 0x1422688c0},
     {"Frpg2RequestMessage::RequestGetSignList", (void*) 0x142262840},
     {"Frpg2RequestMessage::RequestGetSignListResponse", (void*) 0x142262b90},
     {"Frpg2RequestMessage::RequestSummonSign", (void*) 0x14226a0a0},
     {"Frpg2RequestMessage::RequestRejectSign", (void*) 0x142267e30},
     {"Frpg2RequestMessage::PushRequestRemoveSign", (void*) 0x142259f90},
     {"Frpg2RequestMessage::PushRequestSummonSign", (void*) 0x14225a310},
     {"Frpg2RequestMessage::PushRequestRejectSign", (void*) 0x142259960},
     {"Frpg2RequestMessage::RequestCreateMark", (void*) 0x14225dc50},
     {"Frpg2RequestMessage::RequestCreateMarkResponse", (void*) 0x14225df60},
     {"Frpg2RequestMessage::RequestRemoveMark", (void*) 0x1422685a0},
     {"Frpg2RequestMessage::RequestReentryMark", (void*) 0x1422670c0},
     {"Frpg2RequestMessage::RequestGetMarkList", (void*) 0x1422608e0},
     {"Frpg2RequestMessage::RequestGetMarkListResponse", (void*) 0x142260ad0},
     {"Frpg2RequestMessage::RequestSendQuickMatchStart", (void*) 0x142269d40},
     {"Frpg2RequestMessage::RequestSendQuickMatchStart_Session_member_list", (void*) 0x142269f60},
     {"Frpg2RequestMessage::RequestSendQuickMatchResult", (void*) 0x1422698a0},
     {"Frpg2RequestMessage::RequestSendQuickMatchResultResponse", (void*) 0x142269c00},
     {"Frpg2RequestMessage::NRLoggingMessage", (void*) 0x142257290},
     {"Frpg2RequestMessage::RequestQueryLoginServerInfo", (void*) 0x142266450},
     {"Frpg2RequestMessage::RequestHandshake", (void*) 0x1422631f0},
     {"Frpg2RequestMessage::GetServiceStatus", (void*) 0x142254f40},
     {"Frpg2RequestMessage::GetServiceStatusResponse", (void*) 0x142255330},
     {"Frpg2RequestMessage::RequestWaitForUserLogin", (void*) 0x14226b340},
     {"Frpg2RequestMessage::RequestWaitForUserLoginResponse", (void*) 0x14226b6b0},
     {"Frpg2RequestMessage::RequestQueryLoginServerInfoResponse", (void*) 0x142266770},
     {"Frpg2RequestMessage::RequestQueryLoginServerInfoForXboxOne", (void*) 0x142266660},
     {"Frpg2RequestMessage::RequestHandshakeResponse", (void*) 0x142263300},
     {"Frpg2RequestMessage::GetServiceStatusForXboxOne", (void*) 0x1422551b0},
     {"Frpg2RequestMessage::LoginForXboxOne", (void*) 0x142256840},
     {"Frpg2RequestMessage::SocketOptionSetting", (void*) 0x14226c8d0},
     {"Frpg2RequestMessage::GetConnectGameServerPortIdResponse", (void*) 0x142254d10},
     {"Frpg2RequestMessage::UserLogout", (void*) 0x14226d100},
     {"Frpg2RequestMessage::DateTime", (void*) 0x142254490},
     {"Frpg2RequestMessage::ManagementTextMessage", (void*) 0x142256950},
     {"Frpg2RequestMessage::NRLogUploadRequest", (void*) 0x142257120},
     {"Frpg2RequestMessage::RequestSendMessageToPlayersResponse", (void*) 0x142269300},
     {"Frpg2RequestMessage::RequestSendMessageToSelectLanguagePlayers", (void*) 0x1422695e0},
     {"Frpg2RequestMessage::RequestSendMessageToSelectLanguagePlayersResponse", (void*) 0x1422697c0},
     {"Frpg2RequestMessage::RequestSendMessageToAllPlayers", (void*) 0x142268ff0},
     {"Frpg2RequestMessage::RequestSendMessageToAllPlayersResponse", (void*) 0x142269100},
     {"Frpg2RequestMessage::RequestSendPlayerList", (void*) 0x142269830},
     {"Frpg2RequestMessage::NotifyClientServerInfoToPushServer", (void*) 0x142257420},
     {"Frpg2RequestMessage::PushLoginUserInfo", (void*) 0x1422585b0},
     {"Frpg2RequestMessage::NotifyLoginToPushServer", (void*) 0x1422575c0},
     {"Frpg2RequestMessage::NotifyLogoutToPushServer", (void*) 0x142257760},
     {"Frpg2RequestMessage::RequestCheckLogin", (void*) 0x14225d110},
     {"Frpg2RequestMessage::UserLoginInfo", (void*) 0x14226cfb0},
     {"Frpg2RequestMessage::RequestCheckLoginResponse", (void*) 0x14225d210},
     {"Frpg2RequestMessage::RequestGetPlayerConnectGameServer", (void*) 0x1422619c0},
     {"Frpg2RequestMessage::PlayerConnectServerInfo", (void*) 0x142257ed0},
     {"Frpg2RequestMessage::RequestGetPlayerConnectGameServerResponse", (void*) 0x142261ac0},
     {"Frpg2RequestMessage::RequestGetServerConnectTime", (void*) 0x1422625a0},
     {"Frpg2RequestMessage::RequestGetServerConnectTimeResponse", (void*) 0x142262680},
     {"Frpg2RequestMessage::RequestGetConnectionCount", (void*) 0x14225fc10},
     {"Frpg2RequestMessage::RequestGetConnectionCountResponse", (void*) 0x14225fc80},
     {"Frpg2RequestMessage::RequestDisconnectUser", (void*) 0x14225e470},
     {"Frpg2RequestMessage::RequestDisconnectAllUser", (void*) 0x14225e400},
     {"Frpg2RequestMessage::RequestSendMessageToPushClient", (void*) 0x142269400},
     {"Frpg2RequestMessage::PushRequestHeader", (void*) 0x1422590d0},
     {"Frpg2RequestMessage::RangedLimit", (void*) 0x14225b510},
     {"Frpg2RequestMessage::PlayerUserIDPair", (void*) 0x142258430},
     {"Frpg2RequestMessage::QuickMatchData", (void*) 0x14225a930},
     {"Frpg2RequestMessage::QuickMatchSearchResult", (void*) 0x14225aef0},
     {"Frpg2RequestMessage::JoinQuickMatchMessage", (void*) 0x142255980},
     {"Frpg2RequestMessage::AcceptQuickMatchMessage", (void*) 0x142252e00},
     {"Frpg2RequestMessage::RejectQuickMatchMessage", (void*) 0x14225c430},
     {"Frpg2RequestMessage::RemoveQuickMatchMessage", (void*) 0x14225c6b0},
     {"Frpg2RequestMessage::SignData", (void*) 0x14226c000},
     {"Frpg2RequestMessage::GetSignResult", (void*) 0x1422555a0},
     {"Frpg2RequestMessage::RemoveSignMessage", (void*) 0x14225c790},
     {"Frpg2RequestMessage::SummonSignMessage", (void*) 0x14226cd60},
     {"Frpg2RequestMessage::RejectSignMessage", (void*) 0x14225c570},
     {"Frpg2RequestMessage::CachedSign", (void*) 0x142253fc0},
     {"Frpg2RequestMessage::RitualMarkData", (void*) 0x14226ba40},
     {"Frpg2RequestMessage::BonfireInfo", (void*) 0x142253cf0},
     {"Frpg2RequestMessage::DefenseInfo", (void*) 0x142254800},
     {"Frpg2RequestMessage::CutRateInfo", (void*) 0x142254100},
     {"Frpg2RequestMessage::ResistanceInfo", (void*) 0x14226b830},
     {"Frpg2RequestMessage::BloodstainInfo", (void*) 0x142253b10},
     {"Frpg2RequestMessage::PlayerCharacterData", (void*) 0x142257bb0},
     {"Frpg2RequestMessage::RequestUpdatePlayerCharacterResponse", (void*) 0x14226aa40},
     {"Frpg2RequestMessage::BloodMessageData", (void*) 0x142253430},
     {"Frpg2RequestMessage::RequestRemoveBloodMessageResponse", (void*) 0x142268530},
     {"Frpg2RequestMessage::RequestEvaluateBloodMessageResponse", (void*) 0x14225e710},
     {"Frpg2RequestMessage::BloodMessageEvaluationData", (void*) 0x142253970},
     {"Frpg2RequestMessage::PushRequestEvaluateBloodMessage", (void*) 0x142258e20},
     {"Frpg2RequestMessage::RequestCreateGhostDataResponse", (void*) 0x14225dbe0},
     {"Frpg2RequestMessage::GhostData", (void*) 0x1422557a0},
     {"Frpg2RequestMessage::RequestGetRegulationFile", (void*) 0x142261f30},
     {"Frpg2RequestMessage::RequestGetRegulationFileResponse", (void*) 0x142262010},
     {"Frpg2RequestMessage::RegulationFileMessage", (void*) 0x14225c080},
     {"Frpg2RequestMessage::RegulationFileDiffData", (void*) 0x14225bd60},
     {"Frpg2RequestMessage::RegulationFileUpdateMessage", (void*) 0x14225c200},
     {"Frpg2RequestMessage::RegulationFileUpdatePushMessage", (void*) 0x14225c300},
     {"Frpg2RequestMessage::AnnounceMessageData", (void*) 0x142253000},
     {"Frpg2RequestMessage::AnnounceMessageDataList", (void*) 0x142253330},
     {"Frpg2RequestMessage::RequestUpdatePlayerStatusResponse", (void*) 0x14226ab70},
     {"Frpg2RequestMessage::BreakInTargetData", (void*) 0x142253e40},
     {"Frpg2RequestMessage::PushRequestRemoveBreakInTarget", (void*) 0x142259d40},
     {"Frpg2RequestMessage::RequestBreakInTargetResponse", (void*) 0x14225d0a0},
     {"Frpg2RequestMessage::RequestRejectBreakInTargetResponse", (void*) 0x142267ad0},
     {"Frpg2RequestMessage::VisitorData", (void*) 0x14226d310},
     {"Frpg2RequestMessage::RequestVisitResponse", (void*) 0x14226b2d0},
     {"Frpg2RequestMessage::RequestRejectVisitResponse", (void*) 0x142268380},
     {"Frpg2RequestMessage::RequestNotifyRingBellResponse", (void*) 0x142265dd0},
     {"Frpg2RequestMessage::PushRequestNotifyRingBell", (void*) 0x142259330},
     {"Frpg2RequestMessage::RequestNotifyDisconnectSessionResponse", (void*) 0x1422647a0},
     {"Frpg2RequestMessage::RequestGetServerSidePlayerStatus", (void*) 0x142262760},
     {"Frpg2RequestMessage::PlayerStatusUploadConfig", (void*) 0x1422582d0},
     {"Frpg2RequestMessage::PlayerInfoUploadConfigPushMessage", (void*) 0x1422580e0},
     {"Frpg2RequestMessage::LoginClientInfo", (void*) 0x1422564e0},
     {"Frpg2RequestMessage::RankingData", (void*) 0x14225b670},
     {"Frpg2RequestMessage::RankingRotationID", (void*) 0x14225bc20},
     {"Frpg2RequestMessage::RankingRecordCount", (void*) 0x14225bae0},
     {"Frpg2RequestMessage::RankingDataPack", (void*) 0x14225b990},
     {"Frpg2RequestMessage::RequestRegisterRankingDataResponse", (void*) 0x1422677f0},
     {"Frpg2RequestMessage::RequestGetOnlineShopItemListResponse_Lineup", (void*) 0x142260ee0},
     {"Frpg2RequestMessage::RequestUnregisterQuickMatchResponse", (void*) 0x14226a570},
     {"Frpg2RequestMessage::RequestUpdateQuickMatchResponse", (void*) 0x14226ada0},
     {"Frpg2RequestMessage::RequestJoinQuickMatchResponse", (void*) 0x142263710},
     {"Frpg2RequestMessage::RequestAcceptQuickMatchResponse", (void*) 0x14225cac0},
     {"Frpg2RequestMessage::RequestRejectQuickMatchResponse", (void*) 0x142267dc0},
     {"Frpg2RequestMessage::RequestUpdateSignResponse", (void*) 0x14226afb0},
     {"Frpg2RequestMessage::RequestRemoveSignResponse", (void*) 0x142268a60},
     {"Frpg2RequestMessage::RequestSummonSignResponse", (void*) 0x14226a2d0},
     {"Frpg2RequestMessage::RequestRejectSignResponse", (void*) 0x1422680a0},
     {"Frpg2RequestMessage::RequestRemoveMarkResponse", (void*) 0x142268850},
     {"Frpg2RequestMessage::RequestReentryMarkResponse", (void*) 0x142267260},
     {"Frpg2RequestMessage::LogCommonInfo", (void*) 0x142256290},
     {"Frpg2RequestMessage::RequestGetRightMatchingArea", (void*) 0x142262120},
     {"Frpg2RequestMessage::RequestGetRightMatchingAreaResponse_Area_info", (void*) 0x142262460},
     {"Frpg2RequestMessage::RequestGetCurrentRank", (void*) 0x14225fd60},
     {"Frpg2RequestMessage::RequestGetCurrentRankResponse", (void*) 0x14225fe40},
     {"Frpg2RequestMessage::QuickMatchRankCache", (void*) 0x14225ac50},
     {"Frpg2RequestMessage::RequestPushSendQuickMatchStart", (void*) 0x1422661e0},
     {"Frpg2RequestMessage::QuickMatchSession", (void*) 0x14225b0d0},
     {"Frpg2RequestMessage::QuickMatchSession_Result_list", (void*) 0x14225b2f0},
     {"Frpg2RequestMessage::QuickMatchApologyPoint", (void*) 0x14225a7d0},
     {"Frpg2PlayerData::DateTime", (void*) 0x142292ce0}
};

static std::unordered_map<const char*, void*> protobufSerializeVTable = {
    { "FdpLogMessage::Vector", (void*)0x140c68840 },
    { "FdpLogMessage::LevelUpLog", (void*)0x140c67c90 },
    { "FdpLogMessage::UseMagicLog", (void*)0x140c686d0 },
    { "FdpLogMessage::UseMagicLog_Use_magic_info_list", (void*)0x140c687f0 },
    { "FdpLogMessage::ActGestureLog", (void*)0x140c673a0 },
    { "FdpLogMessage::ActGestureLog_Use_gesture_info_list", (void*)0x140c674c0 },
    { "FdpLogMessage::BrokenEquipmentLog", (void*)0x140c67510 },
    { "FdpLogMessage::UseItemLog", (void*)0x140c68540 },
    { "FdpLogMessage::UseItemLog_Use_item_info_list", (void*)0x140c68660 },
    { "FdpLogMessage::PurchaseItemLog", (void*)0x140c67db0 },
    { "FdpLogMessage::PurchaseItemLog_Purchase_item_info_list", (void*)0x140c67e20 },
    { "FdpLogMessage::GetItemLog", (void*)0x140c67990 },
    { "FdpLogMessage::GetItemLog_Get_item_info_list", (void*)0x140c67a00 },
    { "FdpLogMessage::DropItemLog", (void*)0x140c67710 },
    { "FdpLogMessage::DropItemLog_Throw_away_item_list", (void*)0x140c67780 },
    { "FdpLogMessage::LeaveItemLog", (void*)0x140c67b70 },
    { "FdpLogMessage::LeaveItemLog_Set_item_info_list", (void*)0x140c67be0 },
    { "FdpLogMessage::SaleItemLog", (void*)0x140c68180 },
    { "FdpLogMessage::SaleItemLog_Sale_item_info_list", (void*)0x140c681f0 },
    { "FdpLogMessage::AccessWarehouseLog", (void*)0x140c672c0 },
    { "FdpLogMessage::AccessWarehouseLog_Access_warehouse_info_list", (void*)0x140c67330 },
    { "FdpLogMessage::StrengthenWeaponLog", (void*)0x140c682a0 },
    { "FdpLogMessage::StrengthenWeaponLog_Strengthen_weapon_info_list", (void*)0x140c68310 },
    { "FdpLogMessage::RepairEquipmentLog", (void*)0x140c680c0 },
    { "FdpLogMessage::RepairEquipmentLog_Repair_equipment_info_list", (void*)0x140c68130 },
    { "FdpLogMessage::EquipMagicLog", (void*)0x140c67830 },
    { "FdpLogMessage::ChaneEstusFlaskNumLog", (void*)0x140c675b0 },
    { "FdpLogMessage::GlobalEventLog", (void*)0x140c67b00 },
    { "FdpLogMessage::CreateBloodMessageLog", (void*)0x140c67610 },
    { "FdpLogMessage::EvaluateBloodMessageLog", (void*)0x140c67840 },
    { "FdpLogMessage::VisitResultLog", (void*)0x140c688b0 },
    { "FdpLogMessage::QuickMatchResultLog", (void*)0x140c67fb0 },
    { "FdpLogMessage::QuickMatchEndLog", (void*)0x140c67ef0 },
    { "FdpLogMessage::ConvertSaveDataLog", (void*)0x140c675c0 },
    { "FdpLogMessage::SystemOptionLog", (void*)0x140c68360 },
    { "FdpLogMessage::SystemOptionLog_Game_option_setting", (void*)0x140c683e0 },
    { "FdpLogMessage::SystemOptionLog_Network_setting", (void*)0x140c68480 },
    { "Frpg2RequestMessage::ServerPing", (void*)0x14227ac90 },
    { "Frpg2RequestMessage::RequestMeasureUploadBandwidth", (void*)0x142278960 },
    { "Frpg2RequestMessage::RequestMeasureUploadBandwidthResponse", (void*)0x1422789b0 },
    { "Frpg2RequestMessage::RequestMeasureDownloadBandwidth", (void*)0x1422788c0 },
    { "Frpg2RequestMessage::RequestMeasureDownloadBandwidthResponse", (void*)0x142278910 },
    { "Frpg2RequestMessage::RequestBenchmarkThroughput", (void*)0x142276d60 },
    { "Frpg2RequestMessage::RequestBenchmarkThroughputResponse", (void*)0x142276dd0 },
    { "Frpg2RequestMessage::RequestGetAnnounceMessageList", (void*)0x1422773e0 },
    { "Frpg2RequestMessage::RequestGetAnnounceMessageListResponse", (void*)0x142277450 },
    { "Frpg2RequestMessage::RequestGetOnlineShopItemList", (void*)0x142277d90 },
    { "Frpg2RequestMessage::RequestGetOnlineShopItemListResponse", (void*)0x142277db0 },
    { "Frpg2RequestMessage::RequestCreateBloodstain", (void*)0x142277060 },
    { "Frpg2RequestMessage::DomainLimitData", (void*)0x142274d60 },
    { "Frpg2RequestMessage::RequestGetBloodstainList", (void*)0x1422776a0 },
    { "Frpg2RequestMessage::RequestGetBloodstainListResponse", (void*)0x142277730 },
    { "Frpg2RequestMessage::RequestGetDeadingGhost", (void*)0x142277a40 },
    { "Frpg2RequestMessage::RequestGetDeadingGhostResponse", (void*)0x142277a90 },
    { "Frpg2RequestMessage::RequestCreateBloodMessage", (void*)0x142276fd0 },
    { "Frpg2RequestMessage::RequestCreateBloodMessageResponse", (void*)0x142277040 },
    { "Frpg2RequestMessage::RequestReCreateBloodMessageList", (void*)0x142279650 },
    { "Frpg2RequestMessage::RequestReCreateBloodMessageList_Blood_message_info_list", (void*)0x142279750 },
    { "Frpg2RequestMessage::RequestReCreateBloodMessageListResponse", (void*)0x1422796e0 },
    { "Frpg2RequestMessage::RequestRemoveBloodMessage", (void*)0x142279d60 },
    { "Frpg2RequestMessage::LocatedBloodMessage", (void*)0x142275250 },
    { "Frpg2RequestMessage::RequestReentryBloodMessage", (void*)0x1422797d0 },
    { "Frpg2RequestMessage::RequestReentryBloodMessageResponse", (void*)0x142279870 },
    { "Frpg2RequestMessage::BloodMessageDomainLimitData", (void*)0x142274860 },
    { "Frpg2RequestMessage::RequestGetBloodMessageList", (void*)0x1422775a0 },
    { "Frpg2RequestMessage::RequestGetBloodMessageListResponse", (void*)0x142277630 },
    { "Frpg2RequestMessage::RequestEvaluateBloodMessage", (void*)0x142277360 },
    { "Frpg2RequestMessage::RequestGetBloodMessageEvaluation", (void*)0x1422774c0 },
    { "Frpg2RequestMessage::RequestGetBloodMessageEvaluationResponse", (void*)0x142277530 },
    { "Frpg2RequestMessage::RequestCreateGhostData", (void*)0x1422770d0 },
    { "Frpg2RequestMessage::RequestGetGhostDataList", (void*)0x142277b00 },
    { "Frpg2RequestMessage::RequestGetGhostDataListResponse", (void*)0x142277b90 },
    { "Frpg2PlayerData::BonfireInfo", (void*)0x142298250 },
    { "Frpg2PlayerData::DefenseInfo", (void*)0x142298470 },
    { "Frpg2PlayerData::CutRateInfo", (void*)0x1422982a0 },
    { "Frpg2PlayerData::ResistanceInfo", (void*)0x1422999d0 },
    { "Frpg2PlayerData::Equipment", (void*)0x142298560 },
    { "Frpg2PlayerData::PlayerLocation", (void*)0x142298e00 },
    { "Frpg2PlayerData::LogInfo", (void*)0x142298ca0 },
    { "Frpg2PlayerData::PlayData", (void*)0x142298d10 },
    { "Frpg2PlayerData::StatsInfo", (void*)0x142299a50 },
    { "Frpg2PlayerData::PlayerStatus", (void*)0x142298e50 },
    { "Frpg2PlayerData::AllStatus", (void*)0x142298140 },
    { "Frpg2RequestMessage::RequestUpdatePlayerCharacter", (void*)0x14227a760 },
    { "Frpg2RequestMessage::RequestUpdatePlayerStatus", (void*)0x14227a7c0 },
    { "Frpg2RequestMessage::MatchingParameter", (void*)0x142275500 },
    { "Frpg2RequestMessage::RequestGetPlayerCharacter", (void*)0x142277f50 },
    { "Frpg2RequestMessage::RequestGetPlayerCharacterResponse", (void*)0x142278080 },
    { "Frpg2RequestMessage::RequestGetLoginPlayerCharacter", (void*)0x142277c00 },
    { "Frpg2RequestMessage::RequestGetLoginPlayerCharacterResponse", (void*)0x142277c20 },
    { "Frpg2RequestMessage::PlayerCharacterID", (void*)0x142275990 },
    { "Frpg2RequestMessage::RequestGetPlayerCharacterList", (void*)0x142277fa0 },
    { "Frpg2RequestMessage::RequestGetPlayerCharacterListResponse", (void*)0x142278010 },
    { "Frpg2RequestMessage::RequestUpdateLoginPlayerCharacter", (void*)0x14227a640 },
    { "Frpg2RequestMessage::RequestUpdateLoginPlayerCharacterResponse", (void*)0x14227a6d0 },
    { "Frpg2RequestMessage::RequestRegisterRankingData", (void*)0x142279a10 },
    { "Frpg2RequestMessage::RequestGetRankingData", (void*)0x1422781d0 },
    { "Frpg2RequestMessage::RequestGetRankingDataResponse", (void*)0x142278240 },
    { "Frpg2RequestMessage::RequestGetCharacterRankingData", (void*)0x1422778f0 },
    { "Frpg2RequestMessage::RequestGetCharacterRankingDataResponse", (void*)0x142277940 },
    { "Frpg2RequestMessage::RequestCountRankingData", (void*)0x142276f90 },
    { "Frpg2RequestMessage::RequestCountRankingDataResponse", (void*)0x142276fb0 },
    { "Frpg2RequestMessage::RequestGetRightMatchingAreaResponse", (void*)0x142278350 },
    { "Frpg2RequestMessage::RequestSendMessageToPlayers", (void*)0x14227a0d0 },
    { "Frpg2RequestMessage::RequestGetBreakInTargetList", (void*)0x1422777a0 },
    { "Frpg2RequestMessage::RequestGetBreakInTargetListResponse", (void*)0x142277850 },
    { "Frpg2RequestMessage::RequestBreakInTarget", (void*)0x142276e20 },
    { "Frpg2RequestMessage::PushRequestBreakInTarget", (void*)0x142275d10 },
    { "Frpg2RequestMessage::PushRequestAllowBreakInTarget", (void*)0x142275c90 },
    { "Frpg2RequestMessage::RequestRejectBreakInTarget", (void*)0x142279aa0 },
    { "Frpg2RequestMessage::PushRequestRejectBreakInTarget", (void*)0x142275f70 },
    { "Frpg2PlayerData::Vector", (void*)0x142299b70 },
    { "Frpg2RequestMessage::SignInfo", (void*)0x14227aea0 },
    { "Frpg2RequestMessage::RequestNotifyRingBell", (void*)0x142279350 },
    { "Frpg2RequestMessage::RequestNotifyDisconnectSession", (void*)0x142278cd0 },
    { "Frpg2RequestMessage::KillerInfo", (void*)0x1422751d0 },
    { "Frpg2RequestMessage::KillEnemyInfo", (void*)0x142275180 },
    { "Frpg2RequestMessage::PartyMemberInfo", (void*)0x142275850 },
    { "Frpg2RequestMessage::RequestNotifyRegisterCharacter", (void*)0x142279220 },
    { "Frpg2RequestMessage::RequestNotifyDie", (void*)0x142278bb0 },
    { "Frpg2RequestMessage::RequestNotifyKillEnemy", (void*)0x142278f40 },
    { "Frpg2RequestMessage::RequestNotifyKillBoss", (void*)0x142278e00 },
    { "Frpg2RequestMessage::RequestNotifyJoinMultiplay", (void*)0x142278d00 },
    { "Frpg2RequestMessage::RequestNotifyLeaveMultiplay", (void*)0x142279080 },
    { "Frpg2RequestMessage::RequestNotifyCreateSignResult", (void*)0x142278aa0 },
    { "Frpg2RequestMessage::RequestNotifySummonSignResult", (void*)0x1422793b0 },
    { "Frpg2RequestMessage::RequestNotifyBreakInResult", (void*)0x1422789d0 },
    { "Frpg2RequestMessage::RequestNotifyProtoBufLog", (void*)0x1422791a0 },
    { "Frpg2RequestMessage::RequestGetVisitorList", (void*)0x142278640 },
    { "Frpg2RequestMessage::RequestGetVisitorListResponse", (void*)0x142278710 },
    { "Frpg2RequestMessage::PushRequestRemoveVisitor", (void*)0x142276250 },
    { "Frpg2RequestMessage::RequestVisit", (void*)0x14227a910 },
    { "Frpg2RequestMessage::PushRequestVisit", (void*)0x142276330 },
    { "Frpg2RequestMessage::RequestRejectVisit", (void*)0x142279cb0 },
    { "Frpg2RequestMessage::PushRequestRejectVisit", (void*)0x1422760d0 },
    { "Frpg2RequestMessage::SignDomainGetInfo", (void*)0x14227ad90 },
    { "Frpg2RequestMessage::SignGetFlags", (void*)0x14227ae30 },
    { "Frpg2RequestMessage::LocatedRitualMark", (void*)0x1422752a0 },
    { "Frpg2RequestMessage::RitualMarkDomainLimit", (void*)0x14227ac20 },
    { "Frpg2RequestMessage::QuickMatchRank", (void*)0x1422764c0 },
    { "Frpg2RequestMessage::RequestSearchQuickMatch", (void*)0x142279ef0 },
    { "Frpg2RequestMessage::RequestSearchQuickMatch_Map_id_list", (void*)0x14227a050 },
    { "Frpg2RequestMessage::RequestSearchQuickMatchResponse", (void*)0x14227a010 },
    { "Frpg2RequestMessage::RequestRegisterQuickMatch", (void*)0x142279960 },
    { "Frpg2RequestMessage::RequestUnregisterQuickMatch", (void*)0x14227a5b0 },
    { "Frpg2RequestMessage::RequestUpdateQuickMatch", (void*)0x14227a810 },
    { "Frpg2RequestMessage::RequestJoinQuickMatch", (void*)0x1422787e0 },
    { "Frpg2RequestMessage::RequestAcceptQuickMatch", (void*)0x142276ce0 },
    { "Frpg2RequestMessage::RequestRejectQuickMatch", (void*)0x142279b50 },
    { "Frpg2RequestMessage::PushRequestJoinQuickMatch", (void*)0x142275e90 },
    { "Frpg2RequestMessage::PushRequestAcceptQuickMatch", (void*)0x142275c30 },
    { "Frpg2RequestMessage::PushRequestRejectQuickMatch", (void*)0x142276010 },
    { "Frpg2RequestMessage::RequestCreateSign", (void*)0x142277210 },
    { "Frpg2RequestMessage::RequestCreateSignResponse", (void*)0x1422772c0 },
    { "Frpg2RequestMessage::RequestUpdateSign", (void*)0x14227a890 },
    { "Frpg2RequestMessage::RequestRemoveSign", (void*)0x142279e70 },
    { "Frpg2RequestMessage::RequestGetSignList", (void*)0x142278470 },
    { "Frpg2RequestMessage::RequestGetSignListResponse", (void*)0x142278600 },
    { "Frpg2RequestMessage::RequestSummonSign", (void*)0x14227a510 },
    { "Frpg2RequestMessage::RequestRejectSign", (void*)0x142279c00 },
    { "Frpg2RequestMessage::PushRequestRemoveSign", (void*)0x1422761f0 },
    { "Frpg2RequestMessage::PushRequestSummonSign", (void*)0x1422762d0 },
    { "Frpg2RequestMessage::PushRequestRejectSign", (void*)0x142276070 },
    { "Frpg2RequestMessage::RequestCreateMark", (void*)0x142277130 },
    { "Frpg2RequestMessage::RequestCreateMarkResponse", (void*)0x1422771f0 },
    { "Frpg2RequestMessage::RequestRemoveMark", (void*)0x142279dc0 },
    { "Frpg2RequestMessage::RequestReentryMark", (void*)0x1422798e0 },
    { "Frpg2RequestMessage::RequestGetMarkList", (void*)0x142277c90 },
    { "Frpg2RequestMessage::RequestGetMarkListResponse", (void*)0x142277d20 },
    { "Frpg2RequestMessage::RequestSendQuickMatchStart", (void*)0x14227a430 },
    { "Frpg2RequestMessage::RequestSendQuickMatchStart_Session_member_list", (void*)0x14227a4c0 },
    { "Frpg2RequestMessage::RequestSendQuickMatchResult", (void*)0x14227a2e0 },
    { "Frpg2RequestMessage::RequestSendQuickMatchResultResponse", (void*)0x14227a3d0 },
    { "Frpg2RequestMessage::NRLoggingMessage", (void*)0x1422756e0 },
    { "Frpg2RequestMessage::RequestQueryLoginServerInfo", (void*)0x142279570 },
    { "Frpg2RequestMessage::RequestHandshake", (void*)0x1422787b0 },
    { "Frpg2RequestMessage::GetServiceStatus", (void*)0x142274e40 },
    { "Frpg2RequestMessage::GetServiceStatusResponse", (void*)0x142274f20 },
    { "Frpg2RequestMessage::RequestWaitForUserLogin", (void*)0x14227a9c0 },
    { "Frpg2RequestMessage::RequestWaitForUserLoginResponse", (void*)0x14227aa90 },
    { "Frpg2RequestMessage::RequestQueryLoginServerInfoResponse", (void*)0x142279600 },
    { "Frpg2RequestMessage::RequestQueryLoginServerInfoForXboxOne", (void*)0x1422795e0 },
    { "Frpg2RequestMessage::RequestHandshakeResponse", (void*)0x1422787d0 },
    { "Frpg2RequestMessage::GetServiceStatusForXboxOne", (void*)0x142274ed0 },
    { "Frpg2RequestMessage::LoginForXboxOne", (void*)0x142275430 },
    { "Frpg2RequestMessage::SocketOptionSetting", (void*)0x14227aef0 },
    { "Frpg2RequestMessage::GetConnectGameServerPortIdResponse", (void*)0x142274db0 },
    { "Frpg2RequestMessage::UserLogout", (void*)0x14227b0f0 },
    { "Frpg2RequestMessage::DateTime", (void*)0x142274ba0 },
    { "Frpg2RequestMessage::ManagementTextMessage", (void*)0x142275450 },
    { "Frpg2RequestMessage::NRLogUploadRequest", (void*)0x142275690 },
    { "Frpg2RequestMessage::RequestSendMessageToPlayersResponse", (void*)0x14227a170 },
    { "Frpg2RequestMessage::RequestSendMessageToSelectLanguagePlayers", (void*)0x14227a250 },
    { "Frpg2RequestMessage::RequestSendMessageToSelectLanguagePlayersResponse", (void*)0x14227a2c0 },
    { "Frpg2RequestMessage::RequestSendMessageToAllPlayers", (void*)0x14227a0a0 },
    { "Frpg2RequestMessage::RequestSendMessageToAllPlayersResponse", (void*)0x14227a0c0 },
    { "Frpg2RequestMessage::RequestSendPlayerList", (void*)0x14227a2d0 },
    { "Frpg2RequestMessage::NotifyClientServerInfoToPushServer", (void*)0x142275730 },
    { "Frpg2RequestMessage::PushLoginUserInfo", (void*)0x142275be0 },
    { "Frpg2RequestMessage::NotifyLoginToPushServer", (void*)0x1422757a0 },
    { "Frpg2RequestMessage::NotifyLogoutToPushServer", (void*)0x142275810 },
    { "Frpg2RequestMessage::RequestCheckLogin", (void*)0x142276eb0 },
    { "Frpg2RequestMessage::UserLoginInfo", (void*)0x14227b0a0 },
    { "Frpg2RequestMessage::RequestCheckLoginResponse", (void*)0x142276f20 },
    { "Frpg2RequestMessage::RequestGetPlayerConnectGameServer", (void*)0x1422780f0 },
    { "Frpg2RequestMessage::PlayerConnectServerInfo", (void*)0x1422759e0 },
    { "Frpg2RequestMessage::RequestGetPlayerConnectGameServerResponse", (void*)0x142278160 },
    { "Frpg2RequestMessage::RequestGetServerConnectTime", (void*)0x142278410 },
    { "Frpg2RequestMessage::RequestGetServerConnectTimeResponse", (void*)0x142278430 },
    { "Frpg2RequestMessage::RequestGetConnectionCount", (void*)0x142277980 },
    { "Frpg2RequestMessage::RequestGetConnectionCountResponse", (void*)0x142277990 },
    { "Frpg2RequestMessage::RequestDisconnectUser", (void*)0x1422772f0 },
    { "Frpg2RequestMessage::RequestDisconnectAllUser", (void*)0x1422772e0 },
    { "Frpg2RequestMessage::RequestSendMessageToPushClient", (void*)0x14227a1e0 },
    { "Frpg2RequestMessage::PushRequestHeader", (void*)0x142275e70 },
    { "Frpg2RequestMessage::RangedLimit", (void*)0x142276780 },
    { "Frpg2RequestMessage::PlayerUserIDPair", (void*)0x142275b90 },
    { "Frpg2RequestMessage::QuickMatchData", (void*)0x142276450 },
    { "Frpg2RequestMessage::QuickMatchSearchResult", (void*)0x1422765b0 },
    { "Frpg2RequestMessage::JoinQuickMatchMessage", (void*)0x1422750c0 },
    { "Frpg2RequestMessage::AcceptQuickMatchMessage", (void*)0x1422745c0 },
    { "Frpg2RequestMessage::RejectQuickMatchMessage", (void*)0x142276bd0 },
    { "Frpg2RequestMessage::RemoveQuickMatchMessage", (void*)0x142276c70 },
    { "Frpg2RequestMessage::SignData", (void*)0x14227acb0 },
    { "Frpg2RequestMessage::GetSignResult", (void*)0x142274fa0 },
    { "Frpg2RequestMessage::RemoveSignMessage", (void*)0x142276c90 },
    { "Frpg2RequestMessage::SummonSignMessage", (void*)0x14227b010 },
    { "Frpg2RequestMessage::RejectSignMessage", (void*)0x142276c20 },
    { "Frpg2RequestMessage::CachedSign", (void*)0x142274a50 },
    { "Frpg2RequestMessage::RitualMarkData", (void*)0x14227ab60 },
    { "Frpg2RequestMessage::BonfireInfo", (void*)0x1422749b0 },
    { "Frpg2RequestMessage::DefenseInfo", (void*)0x142274c70 },
    { "Frpg2RequestMessage::CutRateInfo", (void*)0x142274aa0 },
    { "Frpg2RequestMessage::ResistanceInfo", (void*)0x14227aae0 },
    { "Frpg2RequestMessage::BloodstainInfo", (void*)0x142274940 },
    { "Frpg2RequestMessage::PlayerCharacterData", (void*)0x142275920 },
    { "Frpg2RequestMessage::RequestUpdatePlayerCharacterResponse", (void*)0x14227a7b0 },
    { "Frpg2RequestMessage::BloodMessageData", (void*)0x142274770 },
    { "Frpg2RequestMessage::RequestRemoveBloodMessageResponse", (void*)0x142279db0 },
    { "Frpg2RequestMessage::RequestEvaluateBloodMessageResponse", (void*)0x1422773d0 },
    { "Frpg2RequestMessage::BloodMessageEvaluationData", (void*)0x1422748d0 },
    { "Frpg2RequestMessage::PushRequestEvaluateBloodMessage", (void*)0x142275dd0 },
    { "Frpg2RequestMessage::RequestCreateGhostDataResponse", (void*)0x142277120 },
    { "Frpg2RequestMessage::GhostData", (void*)0x142275050 },
    { "Frpg2RequestMessage::RequestGetRegulationFile", (void*)0x1422782b0 },
    { "Frpg2RequestMessage::RequestGetRegulationFileResponse", (void*)0x1422782d0 },
    { "Frpg2RequestMessage::RegulationFileMessage", (void*)0x142276ab0 },
    { "Frpg2RequestMessage::RegulationFileDiffData", (void*)0x1422769c0 },
    { "Frpg2RequestMessage::RegulationFileUpdateMessage", (void*)0x142276b00 },
    { "Frpg2RequestMessage::RegulationFileUpdatePushMessage", (void*)0x142276b70 },
    { "Frpg2RequestMessage::AnnounceMessageData", (void*)0x142274630 },
    { "Frpg2RequestMessage::AnnounceMessageDataList", (void*)0x142274700 },
    { "Frpg2RequestMessage::RequestUpdatePlayerStatusResponse", (void*)0x14227a800 },
    { "Frpg2RequestMessage::BreakInTargetData", (void*)0x142274a00 },
    { "Frpg2RequestMessage::PushRequestRemoveBreakInTarget", (void*)0x142276170 },
    { "Frpg2RequestMessage::RequestBreakInTargetResponse", (void*)0x142276ea0 },
    { "Frpg2RequestMessage::RequestRejectBreakInTargetResponse", (void*)0x142279b40 },
    { "Frpg2RequestMessage::VisitorData", (void*)0x14227b170 },
    { "Frpg2RequestMessage::RequestVisitResponse", (void*)0x14227a9b0 },
    { "Frpg2RequestMessage::RequestRejectVisitResponse", (void*)0x142279d50 },
    { "Frpg2RequestMessage::RequestNotifyRingBellResponse", (void*)0x1422793a0 },
    { "Frpg2RequestMessage::PushRequestNotifyRingBell", (void*)0x142275ef0 },
    { "Frpg2RequestMessage::RequestNotifyDisconnectSessionResponse", (void*)0x142278cf0 },
    { "Frpg2RequestMessage::RequestGetServerSidePlayerStatus", (void*)0x142278450 },
    { "Frpg2RequestMessage::PlayerStatusUploadConfig", (void*)0x142275af0 },
    { "Frpg2RequestMessage::PlayerInfoUploadConfigPushMessage", (void*)0x142275a60 },
    { "Frpg2RequestMessage::LoginClientInfo", (void*)0x142275370 },
    { "Frpg2RequestMessage::RankingData", (void*)0x1422767d0 },
    { "Frpg2RequestMessage::RankingRotationID", (void*)0x142276970 },
    { "Frpg2RequestMessage::RankingRecordCount", (void*)0x142276920 },
    { "Frpg2RequestMessage::RankingDataPack", (void*)0x142276890 },
    { "Frpg2RequestMessage::RequestRegisterRankingDataResponse", (void*)0x142279a90 },
    { "Frpg2RequestMessage::RequestGetOnlineShopItemListResponse_Lineup", (void*)0x142277e40 },
    { "Frpg2RequestMessage::RequestUnregisterQuickMatchResponse", (void*)0x14227a630 },
    { "Frpg2RequestMessage::RequestUpdateQuickMatchResponse", (void*)0x14227a880 },
    { "Frpg2RequestMessage::RequestJoinQuickMatchResponse", (void*)0x1422788b0 },
    { "Frpg2RequestMessage::RequestAcceptQuickMatchResponse", (void*)0x142276d50 },
    { "Frpg2RequestMessage::RequestRejectQuickMatchResponse", (void*)0x142279bf0 },
    { "Frpg2RequestMessage::RequestUpdateSignResponse", (void*)0x14227a900 },
    { "Frpg2RequestMessage::RequestRemoveSignResponse", (void*)0x142279ee0 },
    { "Frpg2RequestMessage::RequestSummonSignResponse", (void*)0x14227a5a0 },
    { "Frpg2RequestMessage::RequestRejectSignResponse", (void*)0x142279ca0 },
    { "Frpg2RequestMessage::RequestRemoveMarkResponse", (void*)0x142279e60 },
    { "Frpg2RequestMessage::RequestReentryMarkResponse", (void*)0x142279950 },
    { "Frpg2RequestMessage::LogCommonInfo", (void*)0x1422752f0 },
    { "Frpg2RequestMessage::RequestGetRightMatchingArea", (void*)0x1422782f0 },
    { "Frpg2RequestMessage::RequestGetRightMatchingAreaResponse_Area_info", (void*)0x1422783c0 },
    { "Frpg2RequestMessage::RequestGetCurrentRank", (void*)0x1422779b0 },
    { "Frpg2RequestMessage::RequestGetCurrentRankResponse", (void*)0x1422779d0 },
    { "Frpg2RequestMessage::QuickMatchRankCache", (void*)0x142276510 },
    { "Frpg2RequestMessage::RequestPushSendQuickMatchStart", (void*)0x1422794d0 },
    { "Frpg2RequestMessage::QuickMatchSession", (void*)0x142276670 },
    { "Frpg2RequestMessage::QuickMatchSession_Result_list", (void*)0x142276700 },
    { "Frpg2RequestMessage::QuickMatchApologyPoint", (void*)0x142276400 },
    { "Frpg2PlayerData::DateTime", (void*)0x1422983a0 }
};

// Prelude struct that stores constant data associated with a hook
struct ProfilerPreludeData {
    const char* zone; // 0x00
    uintptr_t original_address; // 0x08
    uintptr_t profiler_entry_address; // 0x10
    uintptr_t profiler_exit_address; // 0x18
    uintptr_t profiler_epilogue_address; // 0x20
    DWORD tls_index; // 0x28
    DWORD _pad; // 0x2C
}; // Total size: 0x30

// Runtime context data associated with a call to a hook
struct ProfilerContextData {
    uintptr_t return_address; // 0x00
    uint64_t rbx; // 0x08
    uint64_t r12; // 0x10
}; // Total size: 0x18

void ProfilingExtension::on_attach()
{
    info("Setting up profiler");

    // Thread local storage to allocate per-thread context stacks
    m_tls_idx = TlsAlloc();
    if (m_tls_idx == TLS_OUT_OF_INDEXES) {
        error("Could not allocate TLS index for profiler");
        return;
    }

    // Hooks main loop to install Optick main loop event
    hooked_MainLoop = register_hook(DS3, 0x140eccb30, tMainLoop);

    // Registers engine created DLThreads with Optick
    hooked_DLThreadHandler = register_hook(DS3, 0x1417ef4b0, tDLThreadHandler);

    for (auto const& [key, val] : protobufDeserializeVTable)
    {
        install_profiler_zone((uintptr_t)val, key);
    }

    for (auto const& [key, val] : protobufSerializeVTable) {
        install_profiler_zone((uintptr_t)val, key);
    }

    // Zones for functions called by the main loop tick
    install_profiler_zone(0x140ee4d10, "fun_140ee4d10");
    install_profiler_zone(0x140e95b35, "fun_140e95b35");
    install_profiler_zone(0x140f01cc0, "fun_140f01cc0");
    install_profiler_zone(0x140e96260, "fun_140e96260");
    install_profiler_zone(0x141779af0, "fun_141779af0");
    //install_profiler_zone(0x14007d5e0, "test_zone");

    // Test SprjGraphicsStep (don't these are actually called though?)
    install_profiler_zone(0x140d4d240, "SprjGraphicsStep::STEP_Init");
    install_profiler_zone(0x140d4e220, "SprjGraphicsStep::STEP_Prepare_forGraphicsFramework");
    install_profiler_zone(0x140d4d370, "SprjGraphicsStep::STEP_Initialize_forGraphicsFramework");
    install_profiler_zone(0x140d4e2a0, "SprjGraphicsStep::STEP_Prepare_forSystem");
    install_profiler_zone(0x140d4e0f0, "SprjGraphicsStep::STEP_Initialize_forSystem");
    install_profiler_zone(0x140d4e240, "SprjGraphicsStep::STEP_Prepare_forGuiFramework");
    install_profiler_zone(0x140d4d450, "SprjGraphicsStep::STEP_Initialize_forGuiFramework");
    install_profiler_zone(0x140d4e260, "SprjGraphicsStep::STEP_Prepare_forRenderingSystem");
    install_profiler_zone(0x140d4d4c0, "SprjGraphicsStep::STEP_Initialize_forRenderingSystem");
    install_profiler_zone(0x140d4e280, "SprjGraphicsStep::STEP_Prepare_forSfxSystem");
    install_profiler_zone(0x140d4e070, "SprjGraphicsStep::STEP_Initialize_forSfxSystem");
    install_profiler_zone(0x140d4e200, "SprjGraphicsStep::STEP_Prepare_forDecalSystem");
    install_profiler_zone(0x140d4d320, "SprjGraphicsStep::STEP_Initialize_forDecalSystem");
    install_profiler_zone(0x140d4c9d0, "SprjGraphicsStep::STEP_BeginDrawStep");
    install_profiler_zone(0x140d4e770, "SprjGraphicsStep::STEP_Update");
    install_profiler_zone(0x140d4e800, "SprjGraphicsStep::STEP_WaitFinishDrawStep");
    install_profiler_zone(0x140d4ca20, "SprjGraphicsStep::STEP_Finish");

    install_profiler_zone(0x140d485b0, "SprjDrawStep::STEP_Update");
    install_profiler_zone(0x140d0f9d0, "SprjFD4LocationStep::STEP_Update");
    install_profiler_zone(0x140980de0, "SprjCameraStep::STEP_Update");
    //install_profiler_zone(0x1409577f0, "SprjWorldScopeStep::STEP_Update");
    install_profiler_zone(0x1404a1820, "SprjLuaEventUpdateTask::_STEP_Update");
    install_profiler_zone(0x14045cd10, "prjEmkSystemUpdateTask::_STEP_Update");
    install_profiler_zone(0x1404b8650, "SprjObjActUpdateTask::_STEP_Update");
    install_profiler_zone(0x140508b10, "SprjFileStep::STEP_Update");
    install_profiler_zone(0x140df18b0, "RendManStep::STEP_Update");
    install_profiler_zone(0x140e3df40, "SprjResStep::STEP_Update");
    install_profiler_zone(0x140e74c10, "SprjSoundStep::STEP_Update");
    install_profiler_zone(0x140e42280, "SprjScaleformStep::STEP_UpdateB");

    install_profiler_zone(0x1400d1cb0, "ForTaskRunner?(fun_1400d1cb0)");
    install_profiler_zone(0x1400b5e80, "GXInternalForTask::Entry?(fun_1400b5e80)");

    // Sync stuff?
    install_profiler_zone(0x1417edb00, "DLKR::DLPlainReadWriteLock::BeginRead");
}

void ProfilingExtension::on_detach()
{
}

void ProfilingExtension::install_profiler_zone(uintptr_t function_address, const char* zone)
{
    unsigned char prelude_code[] = {
        0x53,                                     // push rbx
        0x48, 0x8D, 0x1D, 0xC8, 0xFF, 0xFF, 0xFF, // lea rbx, [rip-56]
        0xFF, 0x63, 0x10,                         // jmp [rbx+16]
    };

    unsigned char prologue_code[] = {
        0x48, 0x8D, 0x1D, 0xBE, 0xFF, 0xFF, 0xFF, // lea rbx, [rip-66]
        0xFF, 0x63, 0x18                          // jmp [rbx+24]
    };

    const auto prelude_data_len = sizeof(struct ProfilerPreludeData);
    const auto prelude_code_len = sizeof(prelude_code);
    const auto prologue_code_len = sizeof(prologue_code);

    void* prelude = VirtualAlloc(0, prelude_data_len + prelude_code_len + prologue_code_len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    void* prelude_trampoline = (void*)((uintptr_t)prelude + prelude_data_len);
    void* prologue_trampoline = (void*)((uintptr_t)prelude_trampoline + prelude_code_len);

    auto hook = register_hook(ALL, function_address, prelude_trampoline);
    reapply();

    struct ProfilerPreludeData prelude_data = {
        //nullptr,
        zone,
        (uintptr_t)hook->original,
        //0,
        (uintptr_t)&profiler_zone,
        (uintptr_t)&profiler_zone_exit,
        (uintptr_t)prologue_trampoline,
        m_tls_idx,
        0
    };

    memcpy(prologue_trampoline, prologue_code, prologue_code_len);
    memcpy(prelude, &prelude_data, prelude_data_len);
    memcpy(prelude_trampoline, prelude_code, prelude_code_len);
}

}

extern "C" void __cdecl __profiler_end(void*)
{
    OPTICK_POP();
}

extern "C" void* __cdecl __profiler_begin(const char* name, void * /*ctx*/)
{
    //spdlog::info("t:{}  {}", GetCurrentThreadId(), ((modengine::ext::ProfilerPreludeData*)ctx)->original_return_address);
    //OPTICK_EVENT(name);
    OPTICK_PUSH_DYNAMIC(name);
    return nullptr;
}
