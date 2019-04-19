# python使用grpc调用rpc接口 - _天枢 - 博客园
## [python使用grpc调用rpc接口](https://www.cnblogs.com/yhleng/p/9512555.html)
2018-08-21 17:11 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9512555)
proto文件：
```
syntax = "proto3";
package coupon;
//
//message UnsetUseC2URequest {
//    int64 bid = 1;
//    int64 uid = 2;
//    int64 sid = 3;
//    int64 transId = 4;
//    repeated int64 c2uIds = 5;
//}
//
//message UnsetUseC2UReply {
//  int64   errCode = 1;
//  string  errMsg  = 2;
//  bool result = 3;
//}
//
////检查发送的券是否可撤销
//message CheckCancelSendC2UByTradeRequest{
//    int64 bid = 1;
//    int64 uid = 2;
//    int64 sid = 3;
//    int64 transId = 4;
//}
//
//message CheckCancelSendC2UByTradeReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3;  //true表示可以撤销。false表示不可以撤销。
//
//}
//
//// 检查交易发送的券是否可撤销
//message CheckCancelSendC2UByConsumeTradeRequest{
//    int64 bid = 1;
//    int64 uid = 2;
//    int64 sid = 3;
//    int64 transId = 4;
//}
//
//message CheckCancelSendC2UByConsumeTradeReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //true表示可以撤销。false表示不可以撤销
//}
//
//message UidAndTransId{
//    int64 uid = 1;
//    int64 transId = 2;
//}
//message CheckCancelSendC2UByTradeMultiRequest{
//    int64 bid = 1;
//    int64 sid = 2;
//    repeated UidAndTransId  params = 3; //json(array(array('uid'=>, 'transId'=>), ..))
//}
//
//message CheckCancelSendC2UByTradeMultiReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; // json 
//}
//message RecoverCouponsByC2uIdsRequest{
//    int64 bid = 1;
//    int64 uid = 2;
//    repeated int64 c2uids = 3;
//    string type = 4;
//}
//
//message RecoverCouponsByC2uIdsReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; //true表示注销成功，false表示注销失败
//}
//
//message UpdateC2URequest{
//   int64 bid = 1;
//   int64 uid = 2;
//   string data = 3; //json
//   string conditions = 4; //json
//}
//message UpdateC2UReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; //true表示操作成功
//}
//
//// 添加券账户信息
//message AddC2URequest{
//    int64 bid = 1;
//    int64 uid = 2;
//    string data = 3; //json
//    string shardKey = 4; //json  可能是无用的 bhy
//}
//
//message AddC2UReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; //true表示操作成功
//}
//
////删除券账户信息
//message DeleteC2URequest{
//    int64 bid = 1;
//    int64 uid = 2;
//    string conditions = 3; //json
//}
//
//message DeleteC2UReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; //true表示操作成功
//}
//
////根据条件获取券账户
//message ListC2UByConditionsRequest{
//    int64 bid = 1;
//    int64 uid = 2;
//    string conditions = 3; //json
//}
//
//message ListC2UByConditionsReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //json
//}
//
////从mysql 获取券账户 
//message ListC2UByMySQLRequest{
//    string conditions = 1; //json
//    string pageOptions = 2; //json
//    string order = 3; //json
//    repeated string fields = 4; //fields
//}
//
//message ListC2UByMySQLReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //json
//}
//
//message ListC2UFutureCanUseRequest{
//   int64 bid = 1;
//   int64 uid = 2;
//   int64 sid = 3;
//   int64 cType = 4;
//   string pageOptions = 5; //json
//   bool isMerge = 6; 
//}
//
//message ListC2UFutureCanUseReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //json
//}
//
//message ListC2UFutureCanUseForPosRequest{
//   int64 bid = 1;
//   int64 uid = 2;
//   int64 sid = 3;
//   int64 cType = 4;
//   string pageOptions = 5; //json
//   bool isMerge = 6; 
//}
//
//message ListC2UFutureCanUseForPosReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //json
//}
//
//message GetC2uForPosConsumeRequest{
//   int64 bid = 1;
//   int64 uid = 2;
//   int64 sid = 3;
//   string pageOptions = 4; //json
//}
//
//message GetC2uForPosConsumeReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //json
//}
//
//message UserGetCouponRequest {
//    int64 bid = 1;
//    int64 uid = 2;
//    string data = 3; //json
//}
//
//message UserGetCouponReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //json
//}
//
//message IsCouponSendComplateRequest {
//    int64 bid = 1;
//    int64 uid = 2;
//    string aids = 3; //json
//}
//
//message IsCouponSendComplateReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3;
//}
//
//message UserCanUseConditionsRequest{
//    int64 bid = 1;
//    int64 uid = 2;
//    string conditions = 3; //json
//    string order = 4; //json
//    string pageOptions = 5; //json
//    bool isMerge = 6; 
//}
//
//message UserCanUseConditionsReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //json
//}
//
//message UserOverCouponRequest{
//    int64 bid = 1;
//    int64 uid = 2;
//    string conditions = 3; //json
//    string pageOptions = 4; //json
//}
//
//message UserOverCouponReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; //json
//}
//
//message UpdateCouponRequest{
//    int64 bid = 1;
//    int64 couponId = 2;
//    string couponData = 3; //json
//}
//
//message UpdateCouponReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message DeleteCouponRequest{
//    int64 bid = 1;
//    int64 couponId = 2;
//}
//
//message DeleteCouponReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message ListAllCouponByConditionsRequest{
//    int64 bid = 1;
//    string conditions = 2; //json
//    string pageOptions = 3; //json
//    string order = 4; //json
//    string fields = 5; //fields
//    int64 sid = 6;
//}
//
//message ListAllCouponByConditionsReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message CountCouponNumsRequest{
//    int64 bid = 1;
//    int64 cType = 2;
//    string cName = 3;
//    int64 sid = 4;
//}
//
//message CountCouponNumsReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    int64 result = 3; 
//}
//
//message CouponQuoteByActivityRequest{
//    int64 bid = 1;
//    int64 aid = 2;
//    repeated int64 couponids = 3; 
//}
//
//message CouponQuoteByActivityReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message CouponUnquoteByActivityRequest{
//    int64 bid = 1;
//    int64 aid = 2;
//    repeated int64 couponids = 3; 
//}
//
//message CouponUnquoteByActivityReply{ 
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message ListCouponsByCouponIdsRequest{
//    int64 bid = 1;
//    repeated int64 couponIds = 2; 
//}
//message ListCouponsByCouponIdsReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; 
//}
//
//message UseMassCouponRequest{
//    int64 bid = 1;
//    string data = 2; //json
//    string condition = 3; //json
//}
//
//message UseMassCouponReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message AddPackRequest{
//    string coupon = 1; //json
//    string subCoupons = 2; //json
//}
//
//message AddPackReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message UpdatePackRequest{
//    int64 bid = 1;
//    int64 packId = 2;
//    string couponData = 3; //json
//}
//
//message UpdatePackReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message checkSubCouponsRequest{
//    int64 bid = 1;
//    int64 packId = 2;
//    string couponIdArray = 3; //json
//}
//
//message checkSubCouponsReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; 
//}
//
//message DeletePackRequest{
//    int64 bid = 1;
//    int64 packId = 2;
//}
//
//message DeletePackReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3; 
//}
//
//message PackRefreshSubCouponsRequest{
//    int64 bid = 1;
//    int64 packId = 2;
//}
//
//message PackRefreshSubCouponsReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; 
//}
//
//message ListCouponByConditionsRequest{
//    string conditions = 1;
//    string pageOptions = 2;
//    string order = 3;
//    string fields = 4;
//}
//
//message ListCouponByConditionsReply{
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3; 
//}
//
//// 查询当前用户所有的优惠券列表(相同券需要合并)
//message ListC2URequest {
//   int64 bid = 1;
//   int64 uid = 2;
//   string conditions = 3; //查询条件json字符串,php调用传json
//     
//}
//
//message ListC2UReply {
//  int64   errCode = 1;
//  string  errMsg  = 2;
//  string  result = 3; //券账户字段json字符串,php调用的时候解析json
//}
//
//// 查询当前用户所有可用的优惠券列表
//message ListC2UCanUseRequest {
//   int64 bid = 1;
//   int64 uid = 2;
//   int64 sid = 3;
//   int64 cType = 4;
//   string pageOptions = 5; //php 传递 json
//   bool isMerge = 6;
//   int64 aid = 7;
//   string from = 8;
//   bool allNotUse = 9; 
//}
//
//message ListC2UCanUseReply {
//  int64   errCode = 1;
//  string  errMsg  = 2;
//  string  result = 3; //券账户字段json字符串,php调用的时候解析json
//}
//
//// 根据状态参数查询当前用户优惠券列表
//message ListC2UCurrentCouponsRequest {
//   int64 bid = 1;
//   int64 uid = 2;
//   int64 sid = 3;
//   int64 cType = 4;
//   string pageOptions = 5; //php 传递 json
//   bool isMerge = 6;
//   int64 aid = 7;
//   string from = 8;
//   bool allNotUse = 9; 
//   int32 useStatus = 10;
//}
//
//message ListC2UCurrentCouponsReply {
//  int64   errCode = 1;
//  string  errMsg  = 2;
//  string  result = 3; //券账户字段json字符串,php调用的时候解析json
//}
//
//// 通过c2uIds 拉取用户账户信息
//message ListC2UByC2UIdsRequest {
//   int64 bid = 1;
//   int64 uid = 2;
//   repeated int64 c2uIds = 3;
//}
//
//message ListC2UByC2UIdsReply {
//    int64   errCode = 1;
//    string  errMsg  = 2;    
//    string result = 3;
//}
message CouponInfo{
    int64 CouponId = 1;
    int64 CouponNum = 2;
    string Code = 3;
}
message SendCouponRequest {
    int64 bid = 1;
    int64 aid = 2;
    int64 sid = 3;
    int64 transId = 4;
    bool fromJob =5;
    repeated int64 uids = 6;
    repeated CouponInfo couponids = 7; 
    int64 obid = 8; // 默认0
    bool isBag = 9; // 是否券包
}
message BoolReply {
    int64 errCode = 1; //0值表示调用接口成功
    string errMsg = 2;
    bool result = 3; 
}
//
////消费使用券
//message UseC2URequest {
//    int64 bid = 1;
//    int64 uid = 2;
//    int64 sid = 3;
//    int64 transId = 4;
//    repeated int64 c2uIds = 5;
//    string tradeType = 6;
//         
//}
//
//message UseC2UReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3;
//}
//
////撤销发送券
//message CancelSendC2UByTradeRequest {
//    int64 bid = 1;
//    int64 uid = 2;
//    int64 sid = 3;
//    int64 transId = 4;
//    string delType = 5;
//    int64 couponsNum = 6;
//    int64  sourceType = 7; 
//    int64 repealTcId = 8;
//
//}
//
//message CancelSendC2UByTradeReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3;
//}
//
////合并用户的券帐户
//message MergeCouponAccountRequest {
//   int64 bid = 1; 
//   int64 fromUid = 2;
//   int64 toUid = 3;
//   int64 mid = 4;
//}
//
//message MergeCouponAccountReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    int32 result = 3;
//}   
//
//message RedisC2USelectRequest {
//   int64 bid = 1;
//   string c2uKey = 2;
//   string fields = 3;
//   string conditions = 4;  
//}
//
//message RedisC2USelectReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3;
//}
//
//message RedisC2UUpdateRequest {
//   int64 bid = 1;
//   string c2uKey = 2;
//   string data = 3;
//   string conditions = 4;  
//}
//
//message RedisC2UUpdateReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3;
//}
//
//message RedisC2URemoveRequest {
//   int64 bid = 1;
//   string c2uKey = 2;
//   string conditions = 3;
//}
//
//message RedisC2URemoveReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3;
//}
//
////添加数据
//message RedisC2UAddRequest {
//   int64 bid = 1;
//   string c2uKey = 2;
//   string data = 3;
//   string conditions = 4;  
//}
//
//message RedisC2UAddReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3;
//}
//
////券模板redis获取
//message RedisSelectCouponRequest {
//   int64 bid = 1;
//   string fields = 2;
//   string conditions = 3;
//}
//
//message RedisSelectCouponReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    string result = 3;
//}
//
//message RedisUpdateCouponRequest {
//   int64 bid = 1;
//   string data = 2;
//   string conditions = 3;  
//}
//
//message RedisUpdateCouponReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3;
//}
//
//message RedisRemoveCouponRequest {
//   int64 bid = 1;
//   string conditions = 2;
//}
//
//message RedisRemoveCouponReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3;
//}
//
////添加数据
//message RedisAddCouponRequest {
//   int64 bid = 1;
//   string data = 2;
//   string conditions = 3;  
//}
//
//message RedisAddCouponReply {
//    int64 errCode = 1; //0值表示调用接口成功
//    string errMsg = 2;
//    bool result = 3;
//}
//
service coupon {
//
//    // 撤销消费使用券
//    rpc UnsetUseC2U (UnsetUseC2URequest) returns (UnsetUseC2UReply) {}
//
//    // 检查发送的券是否可撤销
//    rpc CheckCancelSendC2UByTrade(CheckCancelSendC2UByTradeRequest) returns (CheckCancelSendC2UByTradeReply) {}
//
//    // 查询当前用户所有的优惠券列表(相同券需要合并)
//    rpc ListC2U (ListC2URequest) returns (ListC2UReply) {}
//
//    // 查询当前用户所有可用的优惠券列表
//    rpc ListC2UCanUse (ListC2UCanUseRequest) returns (ListC2UCanUseReply) {}
//
//    // 通过c2uIds 拉取用户账户信息
//    rpc ListC2UByC2UIds(ListC2UByC2UIdsRequest) returns (ListC2UByC2UIdsReply) {}
//
//    // 检查交易发送的券是否可撤销
//    rpc CheckCancelSendC2UByConsumeTrade (CheckCancelSendC2UByConsumeTradeRequest) returns (CheckCancelSendC2UByConsumeTradeReply) {}
//
//    // 检查发送的券是否可撤销
//    rpc CheckCancelSendC2UByTradeMulti (CheckCancelSendC2UByTradeMultiRequest) returns (CheckCancelSendC2UByTradeMultiReply) {}
//
//    // 从指定用户账户中收回指定券 - 已使用的券无法被收回，将自动跳过
//    rpc RecoverCouponsByC2uIds (RecoverCouponsByC2uIdsRequest) returns (RecoverCouponsByC2uIdsReply) {}
//
//    // 更新券账户信息
//    rpc UpdateC2U (UpdateC2URequest) returns (UpdateC2UReply) {}
//
//    // 添加券账户信息
//    rpc AddC2U (AddC2URequest) returns (AddC2UReply) {}
//
//    // 删除券账户
//    rpc DeleteC2U (DeleteC2URequest) returns (DeleteC2UReply) {}
//
//    // 根据条件获取券账户
//    rpc ListC2UByConditions (ListC2UByConditionsRequest) returns (ListC2UByConditionsReply) {}
//
//    // 根据条件获取券账户 从mysql 直接获取
//    rpc ListC2UByMySQL (ListC2UByMySQLRequest) returns (ListC2UByMySQLReply) {}
//
//    // 查询当前用户所有未来可用的优惠券列表 
//    rpc ListC2UFutureCanUse (ListC2UFutureCanUseRequest) returns (ListC2UFutureCanUseReply) {}
//
//    // 查询当前用户所有未来可用的优惠券列表
//    rpc ListC2UFutureCanUseForPos (ListC2UFutureCanUseForPosRequest) returns (ListC2UFutureCanUseForPosReply) {}
//
//    // 软pos消费页面Pos_ConsumeController的indexAction调用的方法
//    rpc GetC2uForPosConsume (GetC2uForPosConsumeRequest) returns (GetC2uForPosConsumeReply) {}
//
//    // 用户领取卡券
//    rpc UserGetCoupon (UserGetCouponRequest) returns (UserGetCouponReply) {}
//
//    // 查看券包有没有发完
//    rpc IsCouponSendComplate(IsCouponSendComplateRequest) returns (IsCouponSendComplateReply) {}
//    // 查询当前用户所有可用的优惠券搜索
//    rpc UserCanUseConditions (UserCanUseConditionsRequest) returns (UserCanUseConditionsReply) {}
//    //userOverCoupon
//    rpc UserOverCoupon (UserOverCouponRequest) returns (UserOverCouponReply) {}
//
//    //coupon:更新券模板
//    rpc UpdateCoupon (UpdateCouponRequest) returns (UpdateCouponReply) {}
//
//    //coupon:删除券模板
//    rpc DeleteCoupon (DeleteCouponRequest) returns (DeleteCouponReply) {}
//
//    //coupon:根据条件获取
//    rpc ListAllCouponByConditions(ListAllCouponByConditionsRequest) returns (ListAllCouponByConditionsReply) {}
//
//    //coupon: 统计优惠券数量
//    rpc CountCouponNums (CountCouponNumsRequest) returns (CountCouponNumsReply) {}
//
//    //coupon:券被活动引用(优惠券cAids字段增加该活动aid) 支持券包，如果是券包，将券包下的子券也标记为引用
//    rpc CouponQuoteByActivity(CouponQuoteByActivityRequest) returns(CouponQuoteByActivityReply) {}
//    //coupon:券被活动取消引用 支持券包，如果是券包，将券包下的子券也取消引用
//    rpc CouponUnquoteByActivity(CouponUnquoteByActivityRequest) returns(CouponUnquoteByActivityReply) {}
//
//    //coupon: 根据couponIds获取券
//    rpc ListCouponsByCouponIds (ListCouponsByCouponIdsRequest) returns (ListCouponsByCouponIdsReply) {}
//
//    //coupon:useMassCoupon
//    rpc UseMassCoupon (UseMassCouponRequest) returns (UseMassCouponReply) {}
//
//    //coupon: 创建卡包
//    rpc AddPack(AddPackRequest) returns (AddPackReply) {}
//
//    //coupon: 更新卡包
//    rpc UpdatePack (UpdatePackRequest) returns (UpdatePackReply) {}
//
//    //coupon:检查子券
//    rpc CheckSubCoupons (checkSubCouponsRequest) returns (checkSubCouponsReply) {}
//
//    //coupon:删除卡包
//    rpc DeletePack (DeletePackRequest) returns (DeletePackReply) {}
//
//    //coupon:刷新券包下的券
//    rpc PackRefreshSubCoupons (PackRefreshSubCouponsRequest) returns (PackRefreshSubCouponsReply) {}
//
//    //coupon:根据条件获取
//    rpc ListCouponByConditions (ListCouponByConditionsRequest) returns (ListCouponByConditionsReply) {}
//
    // SendCoupon : 发券
    rpc SendCoupon (SendCouponRequest) returns (BoolReply);
//
//    //UseC2U 消费使用券
//    rpc UseC2U (UseC2URequest) returns (UseC2UReply) {}
//
//    //CancelSendC2UByTrade 撤销发送券
//    rpc CancelSendC2UByTrade (CancelSendC2UByTradeRequest) returns (CancelSendC2UByTradeReply) {}
//
//    //合并用户的券帐户
//    rpc MergeCouponAccount (MergeCouponAccountRequest) returns (MergeCouponAccountReply) {}
//
//    //redis添加券账户
//    rpc RedisC2UAdd(RedisC2UAddRequest) returns (RedisC2UAddReply) {}
//
//    //redis查询券账户
//    rpc RedisC2USelect(RedisC2USelectRequest) returns (RedisC2USelectReply) {}
//
//    //redis更新券账户
//    rpc RedisC2UUpdate(RedisC2UUpdateRequest) returns (RedisC2UUpdateReply) {}
//
//    //redis删除券账户
//    rpc RedisC2URemove(RedisC2URemoveRequest) returns (RedisC2URemoveReply) {}
//
//    // 根据状态查询当前用户的优惠券列表
//    rpc ListC2UCurrentCoupons(ListC2UCurrentCouponsRequest) returns (ListC2UCurrentCouponsReply) {}
//
//    //券模板获取
//    rpc RedisSelectCoupon(RedisSelectCouponRequest) returns (RedisSelectCouponReply) {}
//
//    //券模板更新
//    rpc RedisUpdateCoupon(RedisUpdateCouponRequest) returns (RedisUpdateCouponReply) {}
//
//    //券模板删除
//    rpc RedisRemoveCoupon(RedisRemoveCouponRequest) returns (RedisRemoveCouponReply) {}
//
//    //券模板添加
//    rpc RedisAddCoupon(RedisAddCouponRequest) returns (RedisAddCouponReply) {}
//
//
}
View Code
```
命令行，生成python rpc架构文件：
```
python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. coupon.proto
```
生成后的两个文件：
coupon_pb2.py
```
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: coupon.proto
import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)
_sym_db = _symbol_database.Default()
DESCRIPTOR = _descriptor.FileDescriptor(
  name='coupon.proto',
  package='coupon',
  syntax='proto3',
  serialized_pb=_b('\n\x0c\x63oupon.proto\x12\x06\x63oupon\"?\n\nCouponInfo\x12\x10\n\x08\x43ouponId\x18\x01 \x01(\x03\x12\x11\n\tCouponNum\x18\x02 \x01(\x03\x12\x0c\n\x04\x43ode\x18\x03 \x01(\t\"\xae\x01\n\x11SendCouponRequest\x12\x0b\n\x03\x62id\x18\x01 \x01(\x03\x12\x0b\n\x03\x61id\x18\x02 \x01(\x03\x12\x0b\n\x03sid\x18\x03 \x01(\x03\x12\x0f\n\x07transId\x18\x04 \x01(\x03\x12\x0f\n\x07\x66romJob\x18\x05 \x01(\x08\x12\x0c\n\x04uids\x18\x06 \x03(\x03\x12%\n\tcouponids\x18\x07 \x03(\x0b\x32\x12.coupon.CouponInfo\x12\x0c\n\x04obid\x18\x08 \x01(\x03\x12\r\n\x05isBag\x18\t \x01(\x08\"<\n\tBoolReply\x12\x0f\n\x07\x65rrCode\x18\x01 \x01(\x03\x12\x0e\n\x06\x65rrMsg\x18\x02 \x01(\t\x12\x0e\n\x06result\x18\x03 \x01(\x08\x32\x44\n\x06\x63oupon\x12:\n\nSendCoupon\x12\x19.coupon.SendCouponRequest\x1a\x11.coupon.BoolReplyb\x06proto3')
)
_COUPONINFO = _descriptor.Descriptor(
  name='CouponInfo',
  full_name='coupon.CouponInfo',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='CouponId', full_name='coupon.CouponInfo.CouponId', index=0,
      number=1, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='CouponNum', full_name='coupon.CouponInfo.CouponNum', index=1,
      number=2, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='Code', full_name='coupon.CouponInfo.Code', index=2,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=24,
  serialized_end=87,
)
_SENDCOUPONREQUEST = _descriptor.Descriptor(
  name='SendCouponRequest',
  full_name='coupon.SendCouponRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='bid', full_name='coupon.SendCouponRequest.bid', index=0,
      number=1, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='aid', full_name='coupon.SendCouponRequest.aid', index=1,
      number=2, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='sid', full_name='coupon.SendCouponRequest.sid', index=2,
      number=3, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='transId', full_name='coupon.SendCouponRequest.transId', index=3,
      number=4, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='fromJob', full_name='coupon.SendCouponRequest.fromJob', index=4,
      number=5, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='uids', full_name='coupon.SendCouponRequest.uids', index=5,
      number=6, type=3, cpp_type=2, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='couponids', full_name='coupon.SendCouponRequest.couponids', index=6,
      number=7, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='obid', full_name='coupon.SendCouponRequest.obid', index=7,
      number=8, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='isBag', full_name='coupon.SendCouponRequest.isBag', index=8,
      number=9, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=90,
  serialized_end=264,
)
_BOOLREPLY = _descriptor.Descriptor(
  name='BoolReply',
  full_name='coupon.BoolReply',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='errCode', full_name='coupon.BoolReply.errCode', index=0,
      number=1, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='errMsg', full_name='coupon.BoolReply.errMsg', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='result', full_name='coupon.BoolReply.result', index=2,
      number=3, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=266,
  serialized_end=326,
)
_SENDCOUPONREQUEST.fields_by_name['couponids'].message_type = _COUPONINFO
DESCRIPTOR.message_types_by_name['CouponInfo'] = _COUPONINFO
DESCRIPTOR.message_types_by_name['SendCouponRequest'] = _SENDCOUPONREQUEST
DESCRIPTOR.message_types_by_name['BoolReply'] = _BOOLREPLY
_sym_db.RegisterFileDescriptor(DESCRIPTOR)
CouponInfo = _reflection.GeneratedProtocolMessageType('CouponInfo', (_message.Message,), dict(
  DESCRIPTOR = _COUPONINFO,
  __module__ = 'coupon_pb2'
  # @@protoc_insertion_point(class_scope:coupon.CouponInfo)
  ))
_sym_db.RegisterMessage(CouponInfo)
SendCouponRequest = _reflection.GeneratedProtocolMessageType('SendCouponRequest', (_message.Message,), dict(
  DESCRIPTOR = _SENDCOUPONREQUEST,
  __module__ = 'coupon_pb2'
  # @@protoc_insertion_point(class_scope:coupon.SendCouponRequest)
  ))
_sym_db.RegisterMessage(SendCouponRequest)
BoolReply = _reflection.GeneratedProtocolMessageType('BoolReply', (_message.Message,), dict(
  DESCRIPTOR = _BOOLREPLY,
  __module__ = 'coupon_pb2'
  # @@protoc_insertion_point(class_scope:coupon.BoolReply)
  ))
_sym_db.RegisterMessage(BoolReply)
_COUPON = _descriptor.ServiceDescriptor(
  name='coupon',
  full_name='coupon.coupon',
  file=DESCRIPTOR,
  index=0,
  options=None,
  serialized_start=328,
  serialized_end=396,
  methods=[
  _descriptor.MethodDescriptor(
    name='SendCoupon',
    full_name='coupon.coupon.SendCoupon',
    index=0,
    containing_service=None,
    input_type=_SENDCOUPONREQUEST,
    output_type=_BOOLREPLY,
    options=None,
  ),
])
_sym_db.RegisterServiceDescriptor(_COUPON)
DESCRIPTOR.services_by_name['coupon'] = _COUPON
# @@protoc_insertion_point(module_scope)
View Code
```
coupon_pb2_grpc.py
```
# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc
import coupon_pb2 as coupon__pb2
class couponStub(object):
  """
  //消费使用券
  message UseC2URequest {
  int64 bid = 1;
  int64 uid = 2;
  int64 sid = 3;
  int64 transId = 4;
  repeated int64 c2uIds = 5;
  string tradeType = 6;
  }
  message UseC2UReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  string result = 3;
  }
  //撤销发送券
  message CancelSendC2UByTradeRequest {
  int64 bid = 1;
  int64 uid = 2;
  int64 sid = 3;
  int64 transId = 4;
  string delType = 5;
  int64 couponsNum = 6;
  int64  sourceType = 7; 
  int64 repealTcId = 8;
  }
  message CancelSendC2UByTradeReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  string result = 3;
  }
  //合并用户的券帐户
  message MergeCouponAccountRequest {
  int64 bid = 1; 
  int64 fromUid = 2;
  int64 toUid = 3;
  int64 mid = 4;
  }
  message MergeCouponAccountReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  int32 result = 3;
  }   
  message RedisC2USelectRequest {
  int64 bid = 1;
  string c2uKey = 2;
  string fields = 3;
  string conditions = 4;  
  }
  message RedisC2USelectReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  string result = 3;
  }
  message RedisC2UUpdateRequest {
  int64 bid = 1;
  string c2uKey = 2;
  string data = 3;
  string conditions = 4;  
  }
  message RedisC2UUpdateReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  message RedisC2URemoveRequest {
  int64 bid = 1;
  string c2uKey = 2;
  string conditions = 3;
  }
  message RedisC2URemoveReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  //添加数据
  message RedisC2UAddRequest {
  int64 bid = 1;
  string c2uKey = 2;
  string data = 3;
  string conditions = 4;  
  }
  message RedisC2UAddReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  //券模板redis获取
  message RedisSelectCouponRequest {
  int64 bid = 1;
  string fields = 2;
  string conditions = 3;
  }
  message RedisSelectCouponReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  string result = 3;
  }
  message RedisUpdateCouponRequest {
  int64 bid = 1;
  string data = 2;
  string conditions = 3;  
  }
  message RedisUpdateCouponReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  message RedisRemoveCouponRequest {
  int64 bid = 1;
  string conditions = 2;
  }
  message RedisRemoveCouponReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  //添加数据
  message RedisAddCouponRequest {
  int64 bid = 1;
  string data = 2;
  string conditions = 3;  
  }
  message RedisAddCouponReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  """
  def __init__(self, channel):
    """Constructor.
    Args:
      channel: A grpc.Channel.
    """
    self.SendCoupon = channel.unary_unary(
        '/coupon.coupon/SendCoupon',
        request_serializer=coupon__pb2.SendCouponRequest.SerializeToString,
        response_deserializer=coupon__pb2.BoolReply.FromString,
        )
class couponServicer(object):
  """
  //消费使用券
  message UseC2URequest {
  int64 bid = 1;
  int64 uid = 2;
  int64 sid = 3;
  int64 transId = 4;
  repeated int64 c2uIds = 5;
  string tradeType = 6;
  }
  message UseC2UReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  string result = 3;
  }
  //撤销发送券
  message CancelSendC2UByTradeRequest {
  int64 bid = 1;
  int64 uid = 2;
  int64 sid = 3;
  int64 transId = 4;
  string delType = 5;
  int64 couponsNum = 6;
  int64  sourceType = 7; 
  int64 repealTcId = 8;
  }
  message CancelSendC2UByTradeReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  string result = 3;
  }
  //合并用户的券帐户
  message MergeCouponAccountRequest {
  int64 bid = 1; 
  int64 fromUid = 2;
  int64 toUid = 3;
  int64 mid = 4;
  }
  message MergeCouponAccountReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  int32 result = 3;
  }   
  message RedisC2USelectRequest {
  int64 bid = 1;
  string c2uKey = 2;
  string fields = 3;
  string conditions = 4;  
  }
  message RedisC2USelectReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  string result = 3;
  }
  message RedisC2UUpdateRequest {
  int64 bid = 1;
  string c2uKey = 2;
  string data = 3;
  string conditions = 4;  
  }
  message RedisC2UUpdateReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  message RedisC2URemoveRequest {
  int64 bid = 1;
  string c2uKey = 2;
  string conditions = 3;
  }
  message RedisC2URemoveReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  //添加数据
  message RedisC2UAddRequest {
  int64 bid = 1;
  string c2uKey = 2;
  string data = 3;
  string conditions = 4;  
  }
  message RedisC2UAddReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  //券模板redis获取
  message RedisSelectCouponRequest {
  int64 bid = 1;
  string fields = 2;
  string conditions = 3;
  }
  message RedisSelectCouponReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  string result = 3;
  }
  message RedisUpdateCouponRequest {
  int64 bid = 1;
  string data = 2;
  string conditions = 3;  
  }
  message RedisUpdateCouponReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  message RedisRemoveCouponRequest {
  int64 bid = 1;
  string conditions = 2;
  }
  message RedisRemoveCouponReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  //添加数据
  message RedisAddCouponRequest {
  int64 bid = 1;
  string data = 2;
  string conditions = 3;  
  }
  message RedisAddCouponReply {
  int64 errCode = 1; //0值表示调用接口成功
  string errMsg = 2;
  bool result = 3;
  }
  """
  def SendCoupon(self, request, context):
    """
    // 撤销消费使用券
    rpc UnsetUseC2U (UnsetUseC2URequest) returns (UnsetUseC2UReply) {}
    // 检查发送的券是否可撤销
    rpc CheckCancelSendC2UByTrade(CheckCancelSendC2UByTradeRequest) returns (CheckCancelSendC2UByTradeReply) {}
    // 查询当前用户所有的优惠券列表(相同券需要合并)
    rpc ListC2U (ListC2URequest) returns (ListC2UReply) {}
    // 查询当前用户所有可用的优惠券列表
    rpc ListC2UCanUse (ListC2UCanUseRequest) returns (ListC2UCanUseReply) {}
    // 通过c2uIds 拉取用户账户信息
    rpc ListC2UByC2UIds(ListC2UByC2UIdsRequest) returns (ListC2UByC2UIdsReply) {}
    // 检查交易发送的券是否可撤销
    rpc CheckCancelSendC2UByConsumeTrade (CheckCancelSendC2UByConsumeTradeRequest) returns (CheckCancelSendC2UByConsumeTradeReply) {}
    // 检查发送的券是否可撤销
    rpc CheckCancelSendC2UByTradeMulti (CheckCancelSendC2UByTradeMultiRequest) returns (CheckCancelSendC2UByTradeMultiReply) {}
    // 从指定用户账户中收回指定券 - 已使用的券无法被收回，将自动跳过
    rpc RecoverCouponsByC2uIds (RecoverCouponsByC2uIdsRequest) returns (RecoverCouponsByC2uIdsReply) {}
    // 更新券账户信息
    rpc UpdateC2U (UpdateC2URequest) returns (UpdateC2UReply) {}
    // 添加券账户信息
    rpc AddC2U (AddC2URequest) returns (AddC2UReply) {}
    // 删除券账户
    rpc DeleteC2U (DeleteC2URequest) returns (DeleteC2UReply) {}
    // 根据条件获取券账户
    rpc ListC2UByConditions (ListC2UByConditionsRequest) returns (ListC2UByConditionsReply) {}
    // 根据条件获取券账户 从mysql 直接获取
    rpc ListC2UByMySQL (ListC2UByMySQLRequest) returns (ListC2UByMySQLReply) {}
    // 查询当前用户所有未来可用的优惠券列表 
    rpc ListC2UFutureCanUse (ListC2UFutureCanUseRequest) returns (ListC2UFutureCanUseReply) {}
    // 查询当前用户所有未来可用的优惠券列表
    rpc ListC2UFutureCanUseForPos (ListC2UFutureCanUseForPosRequest) returns (ListC2UFutureCanUseForPosReply) {}
    // 软pos消费页面Pos_ConsumeController的indexAction调用的方法
    rpc GetC2uForPosConsume (GetC2uForPosConsumeRequest) returns (GetC2uForPosConsumeReply) {}
    // 用户领取卡券
    rpc UserGetCoupon (UserGetCouponRequest) returns (UserGetCouponReply) {}
    // 查看券包有没有发完
    rpc IsCouponSendComplate(IsCouponSendComplateRequest) returns (IsCouponSendComplateReply) {}
    // 查询当前用户所有可用的优惠券搜索
    rpc UserCanUseConditions (UserCanUseConditionsRequest) returns (UserCanUseConditionsReply) {}
    //userOverCoupon
    rpc UserOverCoupon (UserOverCouponRequest) returns (UserOverCouponReply) {}
    //coupon:更新券模板
    rpc UpdateCoupon (UpdateCouponRequest) returns (UpdateCouponReply) {}
    //coupon:删除券模板
    rpc DeleteCoupon (DeleteCouponRequest) returns (DeleteCouponReply) {}
    //coupon:根据条件获取
    rpc ListAllCouponByConditions(ListAllCouponByConditionsRequest) returns (ListAllCouponByConditionsReply) {}
    //coupon: 统计优惠券数量
    rpc CountCouponNums (CountCouponNumsRequest) returns (CountCouponNumsReply) {}
    //coupon:券被活动引用(优惠券cAids字段增加该活动aid) 支持券包，如果是券包，将券包下的子券也标记为引用
    rpc CouponQuoteByActivity(CouponQuoteByActivityRequest) returns(CouponQuoteByActivityReply) {}
    //coupon:券被活动取消引用 支持券包，如果是券包，将券包下的子券也取消引用
    rpc CouponUnquoteByActivity(CouponUnquoteByActivityRequest) returns(CouponUnquoteByActivityReply) {}
    //coupon: 根据couponIds获取券
    rpc ListCouponsByCouponIds (ListCouponsByCouponIdsRequest) returns (ListCouponsByCouponIdsReply) {}
    //coupon:useMassCoupon
    rpc UseMassCoupon (UseMassCouponRequest) returns (UseMassCouponReply) {}
    //coupon: 创建卡包
    rpc AddPack(AddPackRequest) returns (AddPackReply) {}
    //coupon: 更新卡包
    rpc UpdatePack (UpdatePackRequest) returns (UpdatePackReply) {}
    //coupon:检查子券
    rpc CheckSubCoupons (checkSubCouponsRequest) returns (checkSubCouponsReply) {}
    //coupon:删除卡包
    rpc DeletePack (DeletePackRequest) returns (DeletePackReply) {}
    //coupon:刷新券包下的券
    rpc PackRefreshSubCoupons (PackRefreshSubCouponsRequest) returns (PackRefreshSubCouponsReply) {}
    //coupon:根据条件获取
    rpc ListCouponByConditions (ListCouponByConditionsRequest) returns (ListCouponByConditionsReply) {}
    SendCoupon : 发券
    //UseC2U 消费使用券
    rpc UseC2U (UseC2URequest) returns (UseC2UReply) {}
    //CancelSendC2UByTrade 撤销发送券
    rpc CancelSendC2UByTrade (CancelSendC2UByTradeRequest) returns (CancelSendC2UByTradeReply) {}
    //合并用户的券帐户
    rpc MergeCouponAccount (MergeCouponAccountRequest) returns (MergeCouponAccountReply) {}
    //redis添加券账户
    rpc RedisC2UAdd(RedisC2UAddRequest) returns (RedisC2UAddReply) {}
    //redis查询券账户
    rpc RedisC2USelect(RedisC2USelectRequest) returns (RedisC2USelectReply) {}
        //redis更新券账户
    rpc RedisC2UUpdate(RedisC2UUpdateRequest) returns (RedisC2UUpdateReply) {}
        //redis删除券账户
    rpc RedisC2URemove(RedisC2URemoveRequest) returns (RedisC2URemoveReply) {}
    // 根据状态查询当前用户的优惠券列表
    rpc ListC2UCurrentCoupons(ListC2UCurrentCouponsRequest) returns (ListC2UCurrentCouponsReply) {}
    //券模板获取
    rpc RedisSelectCoupon(RedisSelectCouponRequest) returns (RedisSelectCouponReply) {}
    //券模板更新
    rpc RedisUpdateCoupon(RedisUpdateCouponRequest) returns (RedisUpdateCouponReply) {}
    //券模板删除
    rpc RedisRemoveCoupon(RedisRemoveCouponRequest) returns (RedisRemoveCouponReply) {}
    //券模板添加
    rpc RedisAddCoupon(RedisAddCouponRequest) returns (RedisAddCouponReply) {}
    """
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')
def add_couponServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'SendCoupon': grpc.unary_unary_rpc_method_handler(
          servicer.SendCoupon,
          request_deserializer=coupon__pb2.SendCouponRequest.FromString,
          response_serializer=coupon__pb2.BoolReply.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'coupon.coupon', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))
View Code
```
自己新建一个文件，来调用接口：
```
# -*- coding: utf-8 -*-
import grpc
import coupon_pb2,coupon_pb2_grpc
_HOST = '60.205.x.x'
_PORT = '28083'
def run():
    conn = grpc.insecure_channel(_HOST + ':' + _PORT,)
    client = coupon_pb2_grpc.couponStub(channel=conn)
　　 #couponInfo是个数组结构体
    info = coupon_pb2.CouponInfo()
    info.CouponId = 12345692#1 券模版id
    info.CouponNum = 1 #发券数量
    #1061693987532739
    args = coupon_pb2.SendCouponRequest()
    args.bid = 1991243684#2589123516
     args.aid = 11111111
    args.sid = 3691394527#222
     args.uids.append(31609394374409511)  #因uids是数组，所以这里单个要用append增加值，多个用extend([111,3333])
    args.couponids.extend([info])  #info是个结构体
    print(args)
    response = client.SendCoupon(args)
    print(response.errCode) #正确，返回0
    print(response.errMsg)
if __name__ == '__main__':
    run()
```
输出结果：
```
D:\python36\python3.exe D:/untitled4/sendCoupon-grpc.py
bid: 1991243684
aid: 11111111
sid: 3691394527
uids: 31609394374409511
couponids {
  CouponId: 12345692
  CouponNum: 1
}
0
Process finished with exit code 0
```
