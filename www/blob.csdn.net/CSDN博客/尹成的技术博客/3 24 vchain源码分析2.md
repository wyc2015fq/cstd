
# 3.24 vchain源码分析2 - 尹成的技术博客 - CSDN博客

2018年11月09日 14:15:56[尹成](https://me.csdn.net/yincheng01)阅读数：50


接下来是合约的第二部分，直接上代码，注释都在代码中
```python
// Contract to sell and distribute VEN tokens
// 分发VEN 代币
contract VENSale is Owned{
    /// chart of stage transition
    ///
    /// deploy   initialize      startTime                            endTime                 finalize
    ///                              | <-earlyStageLasts-> |             | <- closedStageLasts -> |
    ///  O-----------O---------------O---------------------O-------------O------------------------O------------>
    ///     Created     Initialized           Early             Normal             Closed            Finalized
    enum Stage {
        NotCreated,
        Created,
        Initialized,
        Early,
        Normal,
        Closed,
        Finalized
    }
    using SafeMath for uint256;
    uint256 public constant totalSupply         = (10 ** 9) * (10 ** 18); // 1 billion VEN, decimals set to 18 // 总量 10亿枚
    uint256 constant privateSupply              = totalSupply * 9 / 100;  // 9% for private ICO 私募
    uint256 constant commercialPlan             = totalSupply * 23 / 100; // 23% for commercial plan 企业投资人
    uint256 constant reservedForTeam            = totalSupply * 5 / 100;  // 5% for team // 团队保留
    uint256 constant reservedForOperations      = totalSupply * 22 / 100; // 22 for operations 持续经营及技术发展+ 商业落地推广
    // 59%
    uint256 public constant nonPublicSupply     = privateSupply + commercialPlan + reservedForTeam + reservedForOperations; // 非公开销售 59%
    // 41%
    uint256 public constant publicSupply = totalSupply - nonPublicSupply; // 公开销售41%
    uint256 public constant officialLimit = 64371825 * (10 ** 18); // 官方销售总量
    uint256 public constant channelsLimit = publicSupply - officialLimit; // 渠道销售总量
    // packed to 256bit
    struct SoldOut {
        uint16 placeholder; // placeholder to make struct pre-alloced
        // amount of tdokens officially sol out.
        // max value of 120bit is about 1e36, it's enough for token amount
        uint120 official; // 官方销售数量
        // 通过渠道销售的代币数量
        uint120 channels; // amount of tokens sold out via channels
    }
    SoldOut soldOut;
    uint256 constant venPerEth = 3500;  // normal exchange rate 与以太币的兑换  1 eth = 3500 ven
    uint256 constant venPerEthEarlyStage = venPerEth + venPerEth * 15 / 100;  // early stage has 15% reward 在早期阶段，因为有奖励 所以 1 eth = 3500 + 3500 * 15% = 4025 ven
    uint constant minBuyInterval = 30 minutes; // each account can buy once in 30 minutes  每个帐户可以在30分钟内购买一次
    uint constant maxBuyEthAmount = 30 ether; // 每个账户最大购买数量为30个eth等值的ven
    VEN ven; // VEN token contract follows ERC20 standard
    address ethVault; // the account to keep received ether  接收以太币的账户
    address venVault; // the account to keep non-public offered VEN tokens  发送非公开提供VEN的账户
    uint public constant startTime = 1503057600; // time to start sale  众筹开始时间
    uint public constant endTime = 1504180800;   // tiem to close sale 众筹结束时间
    uint public constant earlyStageLasts = 3 days; // early bird stage lasts in seconds 早期阶段的持续时间
    bool initialized;  // 初始化
    bool finalized; // 结束
    function VENSale() {
        soldOut.placeholder = 1;
    }
    /// @notice calculte exchange rate according to current stage
    /// @return exchange rate. zero if not in sale.
    // 计算当前阶段的兑换比例
    function exchangeRate() constant returns (uint256){
        if (stage() == Stage.Early) {
            return venPerEthEarlyStage; // 早期阶段的比例
        }
        if (stage() == Stage.Normal) {
            return venPerEth; // 正常阶段的比例
        }
        return 0;
    }
    /// @notice for test purpose
    //  区块产生时间
    function blockTime() constant returns (uint32) {
        return uint32(block.timestamp);
    }
    /// @notice estimate stage
    /// @return current stage
    //  得到当前处于哪一个阶段
    function stage() constant returns (Stage) {
        if (finalized) {
            return Stage.Finalized;
        }
        if (!initialized) {
            // deployed but not initialized // 合约刚刚部署
            return Stage.Created;
        }
        // 尚未开始众筹
        if (blockTime() < startTime) {
            // not started yet
            return Stage.Initialized;
        }
        // 售罄
        if (uint256(soldOut.official).add(soldOut.channels) >= publicSupply) {
            // all sold out
            return Stage.Closed;
        }
        // 众筹阶段
        if (blockTime() < endTime) {
            // in sale
            if (blockTime() < startTime.add(earlyStageLasts)) {
            // early bird stage
                return Stage.Early;
            }
            // normal stage
            return Stage.Normal;
        }
        // closed
        return Stage.Closed;
    }
    // 检查是否是一个合约地址
    function isContract(address _addr) constant internal returns(bool) {
        uint size;
        if (_addr == 0) return false;
        assembly {
            size := extcodesize(_addr)
        }
        return size > 0;
    }
    /// @notice entry to buy tokens
    function () payable {
        buy();
    }
    /// @notice entry to buy tokens
    // 买币
    function buy() payable {
        // reject contract buyer to avoid breaking interval limit
        // 禁止合约账户购买代币
        require(!isContract(msg.sender));
        require(msg.value >= 0.01 ether);
        uint256 rate = exchangeRate(); //获取兑换比例
        // here don't need to check stage. rate is only valid when in sale
        require(rate > 0);
        // each account is allowed once in minBuyInterval
        require(blockTime() >= ven.lastMintedTimestamp(msg.sender) + minBuyInterval);// 指定的时间内只允许买一次
        uint256 requested;
        // and limited to maxBuyEthAmount
        // 限制购买数量
        if (msg.value > maxBuyEthAmount) {
            requested = maxBuyEthAmount.mul(rate); // 超过允许购买数量 ， 就使用最大允许购买值乘以兑换率
        } else {
            requested = msg.value.mul(rate);
        }
        uint256 remained = officialLimit.sub(soldOut.official);
        if (requested > remained) { // 要购买的数量 大于剩余数量
            //exceed remained
            requested = remained;
        }
        uint256 ethCost = requested.div(rate); // 计算花费的以太币
        if (requested > 0) {
            ven.mint(msg.sender, requested, true, blockTime());
            // transfer ETH to vault
            ethVault.transfer(ethCost); // 转入以太币
            soldOut.official = requested.add(soldOut.official).toUINT120(); // 修改官方销售的数量
            onSold(msg.sender, requested, ethCost);
        }
        uint256 toReturn = msg.value.sub(ethCost); // (简单来说，此处就是)找零
        if(toReturn > 0) {
            // return over payed ETH
            msg.sender.transfer(toReturn);
        }
    }
    /// @notice returns tokens sold officially
    // 返回正式销售的代币
    function officialSold() constant returns (uint256) {
        return soldOut.official;
    }
    /// @notice returns tokens sold via channels
    // 通过渠道销售的币
    function channelsSold() constant returns (uint256) {
        return soldOut.channels;
    }
    /// @notice manually offer tokens to channel // 手动为渠道提供token
    function offerToChannel(address _channelAccount, uint256 _venAmount) onlyOwner {
        Stage stg = stage(); // 获取当前处于哪个阶段
        // since the settlement may be delayed, so it's allowed in closed stage
        require(stg == Stage.Early || stg == Stage.Normal || stg == Stage.Closed);
        soldOut.channels = _venAmount.add(soldOut.channels).toUINT120(); // 修改渠道销售的token数量
        //should not exceed limit  不能超过渠道销售数量的限制
        require(soldOut.channels <= channelsLimit);
        ven.mint(
            _channelAccount,
            _venAmount,
            true,  // unsold tokens can be claimed by channels portion 未售出的代币可以通过渠道部分来申领
            blockTime()
        );
        onSold(_channelAccount, _venAmount, 0);
    }
    /// @notice initialize to prepare for sale
    /// @param _ven The address VEN token contract following ERC20 standard
    /// @param _ethVault The place to store received ETH
    /// @param _venVault The place to store non-publicly supplied VEN tokens
    //  初始化
    function initialize(
        VEN _ven, // 合约实例
        address _ethVault,  // 接收买家以太币的地址
        address _venVault) onlyOwner { // 发送ven的地址
        require(stage() == Stage.Created); //阶段限制
        // ownership of token contract should already be this
        require(_ven.owner() == address(this)); // 所有权限制
        require(address(_ethVault) != 0); // 不能为空
        require(address(_venVault) != 0);
        ven = _ven;
        ethVault = _ethVault;
        venVault = _venVault;
        // 团队保留铸币
        ven.mint(
            venVault,
            reservedForTeam.add(reservedForOperations),
            false, // team and operations reserved portion can't share unsold tokens
            blockTime()
        );
        // 私募和个人投资人的币
        ven.mint(
            venVault,
            privateSupply.add(commercialPlan),
            true, // private ICO and commercial plan can share unsold tokens 可以共享未出售的token
            blockTime()
        );
        initialized = true;
        onInitialized();
    }
    /// @notice finalize
    function finalize() onlyOwner {
        // only after closed stage
        require(stage() == Stage.Closed); // 阶段限制
        uint256 unsold = publicSupply.sub(soldOut.official).sub(soldOut.channels); //　计算未出售的代币
        if (unsold > 0) {
            // unsold VEN as bonus
            ven.offerBonus(unsold);　// 将未出售的代币作为奖励
        }
        ven.seal();
        finalized = true;
        onFinalized();
    }
    event onInitialized(); // 初始化事件
    event onFinalized(); // 完成事件
    event onSold(address indexed buyer, uint256 venAmount, uint256 ethCost); // 事件 表示有人购买ven
}
```
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

