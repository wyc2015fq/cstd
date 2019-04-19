# 使用 Redis 解决“树”形数据的复杂查询 - 文章 - 伯乐在线
原文出处： [qiujiayu](https://my.oschina.net/u/1469495/blog/1504818)
最近因业务需要，研究了一下树数据结果的存储及查询解决方案。 最初的想法是使用neo4j,可是在网上看了一下开源的不支持集群，感觉用的人不多。
网上也查了一些 [树形结构数据存储方案](http://blog.jobbole.com/112309/) 但每种实现方案都有它的一定局限性。
想了一短时间后，想出了下面的方案：
一、 因为复杂的查询都由Redis来处理，所以数据库表的设计就变得非常简单：tree 表
| 字段名称 | 数据类型 | 备注说明 | | —- | —- | —- | | id | int | 主键 | | parent_id | int | 上级节点ID |
二、Redis的数据存储方案：
把表的数据存储到一个Hash表中，使用表中的id值做为此hash表的key, value值为：


```
{
   id: 10,
   parentId: 9,
   childIds: [11]
}
```
### 代码实现
为了简化测试，这里只演示Redis相关的操作
- Tree 类定义


```
public class Tree {
     private Integer id;
     private String name;
     private Integer parentId;
     private List<Integer> childIds;
 }
```
- 往Redis中添加测试数据：


```
[@Test](https://my.oschina.net/azibug)
 public void addTestData() throws Exception {
     String key = "tree-test-key";
     Tree tree = new Tree();
     List<Integer> childIds = new ArrayList<>();
     int max = 100000
     tree.setChildIds(childIds);
     for (int i = 0; i < max; i++) {
         tree.setId(i);
         tree.setName("tree" + i);
         if (i > 0) {
             tree.setParentId(i - 1);
         }
         childIds.clear();
         if(i < (max - 1)){
             childIds.add(i + 1);
         }
         redis.setHash(key, "" + i, JsonUtil.toJson(tree));
     }
 }
```
- Lua 代码的实现
在Lua中使用递归时，需要使用“尾调用”来优化代码。关于尾调用的知识，大家可以上网去搜索。
#### 获取所有子节点 get-tree-childs.lua


```
local treeKey = KEYS[1]
local fnodeId  = ARGV[1]
local function getTreeChild(currentnode, t, res)
  if currentnode == nil or t == nil  then
    return res
  end
  local nextNode = nil
  local nextType = nil
  if t == "id" and (type(currentnode) == "number" or type(currentnode) == "string") then
    local treeNode = redis.call("HGET", treeKey, currentnode)
    if treeNode then
      local node = cjson.decode(treeNode)
      table.insert(res, treeNode)
      if node and node.childIds then
        nextNode = node.childIds
        nextType = "childIds"
      end
    end
  elseif t == "childIds" then
    nextNode = {}
    nextType = "childIds"
    local treeNode  = nil
    local node = nil
    local cnt = 0
    for _, val in ipairs(currentnode) do
      treeNode = redis.call("HGET", treeKey, tostring(val))
      if treeNode then
        node = cjson.decode(treeNode)
        table.insert(res, treeNode)
        if node and node.childIds then
          for _, val2 in ipairs(node.childIds) do
            table.insert(nextNode, val2)
            cnt = cnt + 1
          end
        end
      end
    end
    if cnt == 0 then
      nextNode = nil
      nextType = nil
    end
  end
  return getTreeChild(nextNode, nextType, res)
end
if treeKey and fnodeId then
  return getTreeChild(fnodeId, "id", {})
end
return {}
```
#### 获取所有子节点数目 get-tree-childs-cnt.lua


```
local treeKey = KEYS[1]
local fnodeId  = ARGV[1]
local function getTreeChildCnt(currentnode, t, res)
  if currentnode == nil or t == nil  then
    return res
  end
  local nextNode = nil
  local nextType = nil
  if t == "id" and (type(currentnode) == "number" or type(currentnode) == "string") then
    local treeNode = redis.call("HGET", treeKey, currentnode)
    if treeNode then
      local node = cjson.decode(treeNode)
      res = res + 1
      if node and node.childIds then
        nextNode = node.childIds
        nextType = "childIds"
      end
    end
  elseif t == "childIds" then
    nextNode = {}
    nextType = "childIds"
    local treeNode  = nil
    local cnt = 0
    for _, val in ipairs(currentnode) do
      treeNode = redis.call("HGET", treeKey, tostring(val))
      if treeNode then
        local node = cjson.decode(treeNode)
        res = res + 1
        if node and node.childIds then
          for _, val2 in ipairs(node.childIds) do
            table.insert(nextNode, val2)
            cnt = cnt + 1
          end
        end
      end
    end
    if cnt == 0 then
      nextNode = nil
      nextType = nil
    end
  end
  return getTreeChildCnt(nextNode, nextType, res)
end
if treeKey and fnodeId then
  return getTreeChildCnt(fnodeId, "id", 0)
end
return 0
```
#### 获取所有子节点数目 get-tree-parent.lua


```
local treeKey = KEYS[1]
local nodeId  = ARGV[1]
local function getTreeParent(treeKey, res, nodeId)
  if nodeId == nil or not (type(nodeId) == "number" or type(nodeId) == "string") then
    return res
  end
  local treeNode = redis.call("HGET", treeKey, nodeId)
  local nextNodeId = nil
  if treeNode then
    local node = cjson.decode(treeNode)
    table.insert(res, treeNode)
    if node then
      nextNodeId = node.parentId
    end
  end
  return getTreeParent(treeKey, res, nextNodeId)
end
if treeKey and nodeId then
  return getTreeParent(treeKey, {}, nodeId)
end
return {}
```
#### 获取所有子节点数目 get-tree-parent-cnt.lua


```
local treeKey = KEYS[1]
local nodeId  = ARGV[1]
local function getTreeParentCnt(treeKey, nodeId, res)
  if nodeId == nil or not (type(nodeId) == "number" or type(nodeId) == "string") then
    return res
  end
  local treeNode = redis.call("HGET", treeKey, nodeId)
  local nextNodeId = nil
  if treeNode then
    local node = cjson.decode(treeNode)
    res = res + 1
    if node then
      nextNodeId = node.parentId
    end
  end
  return getTreeParentCnt(treeKey, nextNodeId, res)
end
if treeKey and nodeId then
  return getTreeParentCnt(treeKey, nodeId, 0)
end
return 0
```
以上代码因为使用了“尾调用”，所以变得相对比较复杂
### 总结
此方案相对比较灵活，能支持相对比较大量的数据。
缺点：过于依赖Redis。数据同步会麻烦些，好在操作不是很复杂。
