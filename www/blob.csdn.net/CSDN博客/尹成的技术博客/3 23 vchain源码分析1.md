
# 3.23 vchain源码分析1 - 尹成的技术博客 - CSDN博客

2018年11月09日 14:14:38[尹成](https://me.csdn.net/yincheng01)阅读数：36


在做vechain智能合约源码分析之前，我们先来看一下它的简介
**一、项目名称**
VeChain(VEN)
**二、项目定位**
全球领先的区块链商品和信息平台
**三、项目简评**
项目旨在解决商业社会中产品的信息和信任问题，利用区块链做高附加值产品的防伪追朔系统，基于此打造信任商业的生态体系。
**四、项目愿景**
vechain的目标其实就是应用区块链技术构造一个既可以自我循环、也可以向外拓展的可信任分布式商业生态环境。
接下来 我们直接通过它的智能合约代码来仔细了解一下它的内部实现细节
首先是第一部分合约代码：
```python
pragma solidity ^0.4.11;
// 权限控制合约
contract Owned {
    address public owner;
    function Owned() {
        owner = msg.sender;
    }
    // 权限控制修饰符
    modifier onlyOwner() {
        require(msg.sender == owner);
        _;
    }
    function setOwner(address _newOwner) onlyOwner {
        owner = _newOwner;
    }
}
/**
 * @title SafeMath
 * @dev Math operations with safety checks that throw on error
 */
// 数学安全函数
library SafeMath {
    function mul(uint256 a, uint256 b) internal constant returns (uint256) {
        uint256 c = a * b;
        assert(a == 0 || c / a == b); // 判断 a不能为0 C/A不能等于b(防止造成gas浪费)
        return c;
    }
    function div(uint256 a, uint256 b) internal constant returns (uint256) {
        // assert(b > 0); // Solidity automatically throws when dividing by 0
        uint256 c = a / b;
        // assert(a == b * c + a % b); // There is no case in which this doesn't hold
        return c;
    }
    function sub(uint256 a, uint256 b) internal constant returns (uint256) {
        assert(b <= a); // 预防溢出
        return a - b;
    }
    function add(uint256 a, uint256 b) internal constant returns (uint256) {
        uint256 c = a + b;
        assert(c >= a); // 节省gas
        return c;
    }
    function toUINT112(uint256 a) internal constant returns(uint112) {
        assert(uint112(a) == a); // 预防溢出
        return uint112(a);
    }
    function toUINT120(uint256 a) internal constant returns(uint120) {
        assert(uint120(a) == a); // 预防溢出
        return uint120(a);
    }
    function toUINT128(uint256 a) internal constant returns(uint128) {
        assert(uint128(a) == a); // 预防溢出
        return uint128(a);
    }
}
// Abstract contract for the full ERC 20 Token standard
// https://github.com/ethereum/EIPs/issues/20
// 抽象合约
contract Token {
    /* This is a slight change to the ERC20 base standard.
    function totalSupply() constant returns (uint256 supply);
    is replaced with:
    uint256 public totalSupply;
    This automatically creates a getter function for the totalSupply.
    This is moved to the base contract since public getter functions are not
    currently recognised as an implementation of the matching abstract
    function by the compiler.
    */
    /// total amount of tokens
    //uint256 public totalSupply;
    function totalSupply() constant returns (uint256 supply); // 总发行量
    /// @param _owner The address from which the balance will be retrieved
    /// @return The balance
    function balanceOf(address _owner) constant returns (uint256 balance); // 查询余额
    /// @notice send `_value` token to `_to` from `msg.sender`
    /// @param _to The address of the recipient
    /// @param _value The amount of token to be transferred
    /// @return Whether the transfer was successful or not
    function transfer(address _to, uint256 _value) returns (bool success); // 转账
    /// @notice send `_value` token to `_to` from `_from` on the condition it is approved by `_from`
    /// @param _from The address of the sender
    /// @param _to The address of the recipient
    /// @param _value The amount of token to be transferred
    /// @return Whether the transfer was successful or not
    function transferFrom(address _from, address _to, uint256 _value) returns (bool success); // 指定源地址转向指定目标地址
    /// @notice `msg.sender` approves `_addr` to spend `_value` tokens
    /// @param _spender The address of the account able to transfer the tokens
    /// @param _value The amount of wei to be approved for transfer
    /// @return Whether the approval was successful or not
    function approve(address _spender, uint256 _value) returns (bool success); // 允许量值设置
    /// @param _owner The address of the account owning tokens
    /// @param _spender The address of the account able to transfer the tokens
    /// @return Amount of remaining tokens allowed to spent
    function allowance(address _owner, address _spender) constant returns (uint256 remaining); // 允许指定地址提取的余额
    event Transfer(address indexed _from, address indexed _to, uint256 _value); // 转账事件
    event Approval(address indexed _owner, address indexed _spender, uint256 _value); // approve 事件
}
/// VEN token, ERC20 compliant
contract VEN is Token, Owned {
    using SafeMath for uint256; // 附着库使用
    string public constant name    = "VeChain Token";  //The Token's name 代币名称
    uint8 public constant decimals = 18;               //Number of decimals of the smallest unit 精度
    string public constant symbol  = "VEN";            //An identifier 标识(符号)
    // packed to 256bit to save gas usage.
    // 打包为256位以节省gas
    struct Supplies {
        // uint128's max value is about 3e38.
        // it's enough to present amount of tokens
        uint128 total; // 总量
        uint128 rawTokens;
    }
    Supplies supplies;
    // Packed to 256bit to save gas usage.
    // 打包成256位以节省gas
    struct Account {
        // uint112's max value is about 5e33.
        // it's enough to present amount of tokens
        uint112 balance;
        // raw token can be transformed into balance with bonus
        // 原始token  可以转移到balance中
        uint112 rawTokens;
        // safe to store timestamp
        uint32 lastMintedTimestamp; // 区块时间
    }
    // Balances for each account
    mapping(address => Account) accounts; // 指定地址的余额
    // Owner of account approves the transfer of an amount to another account
    // 账户所有者所允许转到另一个地址的金额
    mapping(address => mapping(address => uint256)) allowed;
    // bonus that can be shared by raw tokens
    uint256 bonusOffered; // 奖励
    // Constructor
    function VEN() {
    }
    // 总发行量
    function totalSupply() constant returns (uint256 supply){
        return supplies.total;
    }
    // Send back ether sent to me
    // 回退函数
    function () {
        revert(); // 将ether返还
    }
    // If sealed, transfer is enabled and mint is disabled
    function isSealed() constant returns (bool) {
        return owner == 0;
    }
    // 最近一次区块生成时间
    function lastMintedTimestamp(address _owner) constant returns(uint32) {
        return accounts[_owner].lastMintedTimestamp;
    }
    // Claim bonus by raw tokens
    function claimBonus(address _owner) internal{
        require(isSealed());
        if (accounts[_owner].rawTokens != 0) {
            uint256 realBalance = balanceOf(_owner); // 真实的余额
            // 奖励
            uint256 bonus = realBalance
            .sub(accounts[_owner].balance)
            .sub(accounts[_owner].rawTokens); // 获取奖励
            accounts[_owner].balance = realBalance.toUINT112();
            accounts[_owner].rawTokens = 0;
            if(bonus > 0){
                Transfer(this, _owner, bonus);
            }
        }
    }
    // What is the balance of a particular account?
    function balanceOf(address _owner) constant returns (uint256 balance) {
        if (accounts[_owner].rawTokens == 0)
            return accounts[_owner].balance;
        if (bonusOffered > 0) {
            uint256 bonus = bonusOffered
            .mul(accounts[_owner].rawTokens)
            .div(supplies.rawTokens);
            return bonus.add(accounts[_owner].balance)
            .add(accounts[_owner].rawTokens);
        }
        return uint256(accounts[_owner].balance)
        .add(accounts[_owner].rawTokens);
    }
    // Transfer the balance from owner's account to another account
    // 转账
    function transfer(address _to, uint256 _amount) returns (bool success) {
        require(isSealed());
        // implicitly claim bonus for both sender and receiver
        claimBonus(msg.sender);
        claimBonus(_to);
        // according to VEN's total supply, never overflow here
        if (accounts[msg.sender].balance >= _amount
        && _amount > 0) {
            accounts[msg.sender].balance -= uint112(_amount);
            accounts[_to].balance = _amount.add(accounts[_to].balance).toUINT112();
            Transfer(msg.sender, _to, _amount);
            return true;
        } else {
            return false;
        }
    }
    // Send _value amount of tokens from address _from to address _to
    // The transferFrom method is used for a withdraw workflow, allowing contracts to send
    // tokens on your behalf, for example to "deposit" to a contract address and/or to charge
    // fees in sub-currencies; the command should fail unless the _from account has
    // deliberately authorized the sender of the message via some mechanism; we propose
    // these standardized APIs for approval:
    function transferFrom(
    address _from,
    address _to,
    uint256 _amount
    ) returns (bool success) {
        require(isSealed());
        // implicitly claim bonus for both sender and receiver
        claimBonus(_from);
        claimBonus(_to);
        // according to VEN's total supply, never overflow here
        if (accounts[_from].balance >= _amount
            && allowed[_from][msg.sender] >= _amount
            && _amount > 0) {
            accounts[_from].balance -= uint112(_amount);
            allowed[_from][msg.sender] -= _amount;
            accounts[_to].balance = _amount.add(accounts[_to].balance).toUINT112();
            Transfer(_from, _to, _amount);
            return true;
        } else {
            return false;
        }
    }
    // Allow _spender to withdraw from your account, multiple times, up to the _value amount.
    // If this function is called again it overwrites the current allowance with _value.
    function approve(address _spender, uint256 _amount) returns (bool success) {
        allowed[msg.sender][_spender] = _amount;
        Approval(msg.sender, _spender, _amount);
        return true;
    }
    /* Approves and then calls the receiving contract */
    function approveAndCall(address _spender, uint256 _value, bytes _extraData) returns (bool success) {
        allowed[msg.sender][_spender] = _value;
        Approval(msg.sender, _spender, _value);
        //call the receiveApproval function on the contract you want to be notified. This crafts the function signature manually so one doesn't have to include a contract in here just for this.
        //receiveApproval(address _from, uint256 _value, address _tokenContract, bytes _extraData)
        //it is assumed that when does this that the call *should* succeed, otherwise one would use vanilla approve instead.
        //if(!_spender.call(bytes4(bytes32(sha3("receiveApproval(address,uint256,address,bytes)"))), msg.sender, _value, this, _extraData)) { revert(); }
        ApprovalReceiver(_spender).receiveApproval(msg.sender, _value, this, _extraData);
        return true;
    }
    function allowance(address _owner, address _spender) constant returns (uint256 remaining) {
        return allowed[_owner][_spender];
    }
    // Mint tokens and assign to some one  铸币
    function mint(address _owner, uint256 _amount, bool _isRaw, uint32 timestamp) onlyOwner{
        if (_isRaw) {
            accounts[_owner].rawTokens = _amount.add(accounts[_owner].rawTokens).toUINT112();
            supplies.rawTokens = _amount.add(supplies.rawTokens).toUINT128();
        } else {
            accounts[_owner].balance = _amount.add(accounts[_owner].balance).toUINT112();
        }
        accounts[_owner].lastMintedTimestamp = timestamp;
        supplies.total = _amount.add(supplies.total).toUINT128();
        Transfer(0, _owner, _amount);
    }
    // Offer bonus to raw tokens holder 给代币持有者提供奖励
    function offerBonus(uint256 _bonus) onlyOwner {
        bonusOffered = bonusOffered.add(_bonus);
        supplies.total = _bonus.add(supplies.total).toUINT128();
        Transfer(0, this, _bonus);
    }
    // Set owner to zero address, to disable mint, and enable token transfer
    function seal() onlyOwner {
        setOwner(0);
    }
}
contract ApprovalReceiver {
    function receiveApproval(address _from, uint256 _value, address _tokenContract, bytes _extraData);
}
```
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

