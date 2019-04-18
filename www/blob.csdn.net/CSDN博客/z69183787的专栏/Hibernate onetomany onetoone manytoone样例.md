# Hibernate onetomany onetoone manytoone样例 - z69183787的专栏 - CSDN博客
2013年12月03日 19:10:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1639
个人分类：[事务-Jpa实践](https://blog.csdn.net/z69183787/article/category/2175487)
数据表：
```
-- Create table
create table T_ASSET
(
  ID                         VARCHAR2(40 CHAR) not null,
  CREATE_DATE                TIMESTAMP(6),
  REMARKS                    VARCHAR2(4000 CHAR),
  UPDATE_DATE                TIMESTAMP(6),
  ASSET_CODE                 VARCHAR2(400 CHAR),
  COUNT                      FLOAT,
  EXPECTANCY_LIFE            NUMBER(19),
  MADE_DATE                  TIMESTAMP(6),
  MANUFACTURE_COUNTRY        VARCHAR2(400 CHAR),
  NAME                       VARCHAR2(400 CHAR),
  TYPE                       VARCHAR2(400 CHAR),
  USE_DATE                   TIMESTAMP(6),
  USE_LIFE                   NUMBER(19),
  WARRANTY_PERIOD            DATE,
  SUB_TYPE_ID                VARCHAR2(40 CHAR),
  MANUFACTURER_ID            VARCHAR2(40 CHAR),
  DETAIL_TYPE_ID             VARCHAR2(40 CHAR),
  ASSET_OWNER_INFO_ID        VARCHAR2(40 CHAR),
  UNIT_ID                    VARCHAR2(40 CHAR),
  MAIN_TYPE_ID               VARCHAR2(40 CHAR),
  STATE_ID                   VARCHAR2(40 CHAR),
  CONTRACT_ID                VARCHAR2(255 CHAR),
  PROJECT_ID                 VARCHAR2(255 CHAR),
  APPROVAL_SCRAP_DATE        TIMESTAMP(6),
  ASSET_PIC                  VARCHAR2(400 CHAR),
  COMPLETE_TRANS_ASSET_TYPE  VARCHAR2(400 CHAR),
  CONTRACT                   VARCHAR2(2000 CHAR),
  DETAIL_INSTALL_SITE        VARCHAR2(2000 CHAR),
  OPERATE_PROJECT_ASSET      VARCHAR2(400 CHAR),
  OPERATE_PROJECT_ASSET_DATE TIMESTAMP(6),
  OVERHAUL_RATE              VARCHAR2(400 CHAR),
  PROJECT_APP_DOC_NO         VARCHAR2(400 CHAR),
  PROJECT_NO                 VARCHAR2(2000 CHAR),
  SUPPLIER_ID                VARCHAR2(40),
  VERIFY_STATE               VARCHAR2(4),
  DATA_LIST                  VARCHAR2(100 CHAR),
  EQUIPMENT_LIST             VARCHAR2(4000 CHAR),
  OVERHAUL_FINAL_PRICE       FLOAT,
  CONTRACT_PRICE             FLOAT,
  DEPRECIATION_METHOD        VARCHAR2(255 CHAR),
  FACTORY_PRICE              FLOAT,
  ORIGINAL_VALUE             FLOAT
)
tablespace USERS
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 7M
    minextents 1
    maxextents unlimited
  );
-- Add comments to the columns 
comment on column T_ASSET.ID
  is '主键';
comment on column T_ASSET.CREATE_DATE
  is '创建时间';
comment on column T_ASSET.REMARKS
  is '备注';
comment on column T_ASSET.UPDATE_DATE
  is '修改时间';
comment on column T_ASSET.ASSET_CODE
  is '资产编码';
comment on column T_ASSET.COUNT
  is '资产数量';
comment on column T_ASSET.EXPECTANCY_LIFE
  is '资产预期使用寿命';
comment on column T_ASSET.MADE_DATE
  is '资产出厂日期';
comment on column T_ASSET.MANUFACTURE_COUNTRY
  is '资产产地';
comment on column T_ASSET.NAME
  is '资产名称';
comment on column T_ASSET.TYPE
  is '规格型号';
comment on column T_ASSET.USE_DATE
  is '资产供应日期';
comment on column T_ASSET.USE_LIFE
  is '设计使用年限';
comment on column T_ASSET.WARRANTY_PERIOD
  is '资产保修期';
comment on column T_ASSET.SUB_TYPE_ID
  is '资产中类ID';
comment on column T_ASSET.MANUFACTURER_ID
  is '资产制造商ID';
comment on column T_ASSET.DETAIL_TYPE_ID
  is '资产小类ID';
comment on column T_ASSET.ASSET_OWNER_INFO_ID
  is '资产权属信息ID';
comment on column T_ASSET.UNIT_ID
  is '资产单位信息ID';
comment on column T_ASSET.MAIN_TYPE_ID
  is '资产大类ID';
comment on column T_ASSET.STATE_ID
  is '资产状态ID';
comment on column T_ASSET.CONTRACT_ID
  is '合同ID';
comment on column T_ASSET.PROJECT_ID
  is '项目ID';
comment on column T_ASSET.APPROVAL_SCRAP_DATE
  is '资产批准报废时间';
comment on column T_ASSET.ASSET_PIC
  is '资产图片名称';
comment on column T_ASSET.COMPLETE_TRANS_ASSET_TYPE
  is '竣工移交资产类型';
comment on column T_ASSET.DETAIL_INSTALL_SITE
  is '资产安装地点';
comment on column T_ASSET.OPERATE_PROJECT_ASSET
  is '项目资产标示';
comment on column T_ASSET.OPERATE_PROJECT_ASSET_DATE
  is '资产交付日期';
comment on column T_ASSET.OVERHAUL_RATE
  is '大修频次';
comment on column T_ASSET.PROJECT_APP_DOC_NO
  is '立项或可研批复文号';
-- Create/Recreate primary, unique and foreign key constraints 
alter table T_ASSET
  add primary key (ID)
  using index 
  tablespace USERS
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 832K
    minextents 1
    maxextents unlimited
  );
alter table T_ASSET
  add constraint FK162C72529B5851C foreign key (UNIT_ID)
  references T_UNIT_MASTER (ID);
alter table T_ASSET
  add constraint FK162C725473A9FA4 foreign key (MAIN_TYPE_ID)
  references T_ASSET_TYPE (ID);
alter table T_ASSET
  add constraint FK162C725900B256A foreign key (STATE_ID)
  references T_ASSET_STATE (ID);
alter table T_ASSET
  add constraint FK162C725A08EF1B4 foreign key (ASSET_OWNER_INFO_ID)
  references T_ASSET_OWNER (ID);
alter table T_ASSET
  add constraint FK162C725A8E19A3C foreign key (DETAIL_TYPE_ID)
  references T_ASSET_TYPE (ID);
alter table T_ASSET
  add constraint FK162C725AC43720A foreign key (MANUFACTURER_ID)
  references T_ENTERPRISE (ID);
alter table T_ASSET
  add constraint FK162C725AD6DBCBA foreign key (PROJECT_ID)
  references T_PROJECT (ID);
alter table T_ASSET
  add constraint FK162C725D374E6DA foreign key (CONTRACT_ID)
  references T_CONTRACT (ID);
alter table T_ASSET
  add constraint FK162C725F8E7E12B foreign key (SUB_TYPE_ID)
  references T_ASSET_TYPE (ID);
alter table T_ASSET
  add constraint FK162C725FB274A6B foreign key (SUPPLIER_ID)
  references T_ENTERPRISE (ID);
```
```
-- Create table
create table T_EQUIPMENTS
(
  ID                  VARCHAR2(40 CHAR) not null,
  CREATE_DATE         TIMESTAMP(6),
  REMARKS             VARCHAR2(255 CHAR),
  UPDATE_DATE         TIMESTAMP(6),
  LOCATION            VARCHAR2(400 CHAR),
  MANUFACTURE_COUNTRY VARCHAR2(400 CHAR),
  NAME                VARCHAR2(400 CHAR),
  PRICE               FLOAT,
  SERVICE_LIFE        FLOAT,
  TYPE                VARCHAR2(400 CHAR),
  USE_DATE            TIMESTAMP(6),
  MANUFACTURER_ID     VARCHAR2(40 CHAR),
  SUPPLIER_ID         VARCHAR2(40 CHAR),
  ASSET_ID            VARCHAR2(40 CHAR)
)
tablespace USERS
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    minextents 1
    maxextents unlimited
  );
-- Create/Recreate primary, unique and foreign key constraints 
alter table T_EQUIPMENTS
  add primary key (ID)
  using index 
  tablespace USERS
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 64K
    minextents 1
    maxextents unlimited
  );
alter table T_EQUIPMENTS
  add constraint FKA8E751703DAE015A foreign key (ASSET_ID)
  references T_ASSET (ID);
alter table T_EQUIPMENTS
  add constraint FKA8E75170762B962F foreign key (SUPPLIER_ID)
  references T_ENTERPRISE (ID);
alter table T_EQUIPMENTS
  add constraint FKA8E75170AC43720A foreign key (MANUFACTURER_ID)
  references T_ENTERPRISE (ID);
```
```
-- Create table
create table T_CONTRACT
(
  ID                          VARCHAR2(255 CHAR) not null,
  BUILD_SUPPLIER_ID           VARCHAR2(400 CHAR),
  BUILD_SUPPLIER_NAME         VARCHAR2(400 CHAR),
  CONTRACT_ATTACHMENT         VARCHAR2(400 CHAR),
  CONTRACT_CONTENT            VARCHAR2(4000 CHAR),
  CONTRACT_DESTORY_DATE       VARCHAR2(400 CHAR),
  CONTRACT_END_DATE           VARCHAR2(400 CHAR),
  CONTRACT_GROUPING           VARCHAR2(400 CHAR),
  CONTRACT_NAME               VARCHAR2(140 CHAR),
  CONTRACT_NO                 VARCHAR2(100 CHAR),
  CONTRACT_OWNER_EXECUTE_ID   VARCHAR2(400 CHAR),
  CONTRACT_OWNER_EXECUTE_NAME VARCHAR2(400 CHAR),
  CONTRACT_OWNER_ID           VARCHAR2(400 CHAR),
  CONTRACT_OWNER_NAME         VARCHAR2(1000 CHAR),
  CONTRACT_PASSED_DATE        VARCHAR2(400 CHAR),
  CONTRACT_PLAN_ID            VARCHAR2(400 CHAR),
  CONTRACT_PLAN_NO            VARCHAR2(400 CHAR),
  CONTRACT_PRICE              VARCHAR2(400 CHAR),
  CONTRACT_SIGNED_DATE        VARCHAR2(400 CHAR),
  CONTRACT_START_DATE         VARCHAR2(400 CHAR),
  CONTRACT_STATUS             VARCHAR2(200 CHAR),
  CONTRACT_TYPE               VARCHAR2(400 CHAR),
  CREATE_DATE                 VARCHAR2(400 CHAR),
  CREATE_TYPE                 VARCHAR2(200 CHAR),
  FINAL_PRICE                 VARCHAR2(400 CHAR),
  INVITE_BID_TYPE             VARCHAR2(400 CHAR),
  NATION_VERIFY_PRICE         VARCHAR2(100 CHAR),
  PAY_TYPE                    VARCHAR2(400 CHAR),
  PROJECT_ID                  VARCHAR2(100 CHAR),
  PROJECT_NAME                VARCHAR2(200 CHAR),
  PROJECT_NO                  VARCHAR2(400 CHAR),
  REGISTER_PERSON_LOGINNAME   VARCHAR2(100 CHAR),
  REGISTER_PERSON_NAME        VARCHAR2(200 CHAR),
  REMARK                      VARCHAR2(4000 CHAR),
  REMOVED                     VARCHAR2(400 CHAR),
  SELF_NO                     VARCHAR2(200 CHAR),
  UPDATE_DATE                 VARCHAR2(400 CHAR),
  VERIFY_PRICE                VARCHAR2(100 CHAR)
)
tablespace USERS
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 5M
    minextents 1
    maxextents unlimited
  );
-- Create/Recreate primary, unique and foreign key constraints 
alter table T_CONTRACT
  add primary key (ID)
  using index 
  tablespace USERS
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 320K
    minextents 1
    maxextents unlimited
  );
```
映射实体类：
```java
package com.wonders.asset.model;
import java.util.Date;
import java.util.Set;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
@Entity
@Table(name="T_ASSET")
public class Asset extends SystemInformation
{
  private String assetCode;
  private String name;
  private Double count;
  private String type;
  private String manufactureCountry;
  private Date madeDate;
  private Date useDate;
  private Long useLife;
  private Long expectancyLife;
  private Date WarrantyPeriod;
  private AssetType mainType;
  private AssetType subType;
  private AssetType DetailType;
  private UnitMaster unit;
  private Enterprise manufacturer;
  private Enterprise supplier;
  private AssetOwner assetOwnerInf;
  private AssetState state;
  private Set<Equipment> equipments;
  private Contract contract;
  private Project project;
  private String detailInstallSite;
  private Date approvalScrapDate;
  private String assetPic;
  private String overhaulRate;
  private String completeTransAssetType;
  private String operateProjectAsset;
  private Date operateProjectAssetDate;
  private String projectAppDocNo;
  private String projectNo;
  private String contractNo;
  private String verifyState;
  private String dataList;
  private String equipmentList;
  private Double overhaulFinalPrice;
  private Double factoryPrice;
  private Double contractPrice;
  private Double originalValue;
  private String depreciationMethod;
  @Column(name="VERIFY_STATE", nullable=true, length=4)
  public String getVerifyState()
  {
    return this.verifyState;
  }
  public void setVerifyState(String verifyState) {
    this.verifyState = verifyState;
  }
  @Column(name="ASSET_CODE", nullable=true, length=400)
  public String getAssetCode() { return this.assetCode; }
  public void setAssetCode(String assetCode) {
    this.assetCode = assetCode;
  }
  @Column(name="NAME", nullable=true, length=400)
  public String getName() {
    return this.name;
  }
  public void setName(String name) {
    this.name = name;
  }
  @Column(name="COUNT", nullable=true)
  public Double getCount() {
    return this.count;
  }
  public void setCount(Double count) {
    this.count = count;
  }
  @Column(name="TYPE", nullable=true, length=400)
  public String getType() {
    return this.type;
  }
  public void setType(String type) {
    this.type = type;
  }
  @Column(name="MANUFACTURE_COUNTRY", nullable=true, length=400)
  public String getManufactureCountry() {
    return this.manufactureCountry;
  }
  public void setManufactureCountry(String manufactureCountry) {
    this.manufactureCountry = manufactureCountry;
  }
  @Column(name="MADE_DATE", nullable=true)
  public Date getMadeDate() {
    return this.madeDate;
  }
  public void setMadeDate(Date madeDate) {
    this.madeDate = madeDate;
  }
  @Column(name="USE_DATE", nullable=true)
  public Date getUseDate() {
    return this.useDate;
  }
  public void setUseDate(Date useDate) {
    this.useDate = useDate;
  }
  @Column(name="USE_LIFE", nullable=true)
  public Long getUseLife() {
    return this.useLife;
  }
  public void setUseLife(Long useLife) {
    this.useLife = useLife;
  }
  @Column(name="EXPECTANCY_LIFE", nullable=true)
  public Long getExpectancyLife() {
    return this.expectancyLife;
  }
  public void setExpectancyLife(Long expectancyLife) {
    this.expectancyLife = expectancyLife;
  }
  @Temporal(TemporalType.DATE)
  @Column(name="WARRANTY_PERIOD", nullable=true, length=400)
  public Date getWarrantyPeriod() { return this.WarrantyPeriod; }
  public void setWarrantyPeriod(Date warrantyPeriod) {
    this.WarrantyPeriod = warrantyPeriod;
  }
  @OneToOne
  @JoinColumn(name="PROJECT_ID")
  public Project getProject() {
    return this.project;
  }
  public void setProject(Project project) {
    this.project = project;
  }
  @OneToOne
  @JoinColumn(name="MAIN_TYPE_ID")
  public AssetType getMainType() { return this.mainType; }
  public void setMainType(AssetType mainType) {
    this.mainType = mainType;
  }
  @OneToOne
  @JoinColumn(name="SUB_TYPE_ID")
  public AssetType getSubType() { return this.subType; }
  public void setSubType(AssetType subType) {
    this.subType = subType;
  }
  @OneToOne
  @JoinColumn(name="DETAIL_TYPE_ID")
  public AssetType getDetailType() { return this.DetailType; }
  public void setDetailType(AssetType detailType) {
    this.DetailType = detailType;
  }
  @OneToOne
  @JoinColumn(name="UNIT_ID")
  public UnitMaster getUnit() { return this.unit; }
  public void setUnit(UnitMaster unit) {
    this.unit = unit;
  }
  @OneToOne
  @JoinColumn(name="MANUFACTURER_ID")
  public Enterprise getManufacturer() { return this.manufacturer; }
  public void setManufacturer(Enterprise manufacturer) {
    this.manufacturer = manufacturer;
  }
  @OneToOne
  @JoinColumn(name="SUPPLIER_ID")
  public Enterprise getSupplier() { return this.supplier; }
  public void setSupplier(Enterprise supplier) {
    this.supplier = supplier; } 
  @OneToOne
  @JoinColumn(name="ASSET_OWNER_INFO_ID")
  public AssetOwner getAssetOwnerInf() { return this.assetOwnerInf; }
  public void setAssetOwnerInf(AssetOwner assetOwnerInf) {
    this.assetOwnerInf = assetOwnerInf;
  }
  @OneToOne
  @JoinColumn(name="STATE_ID")
  public AssetState getState()
  {
    return this.state;
  }
  public void setState(AssetState state) {
    this.state = state;
  }
  @OneToMany(cascade={javax.persistence.CascadeType.ALL}, fetch=FetchType.LAZY, mappedBy="asset")
  public Set<Equipment> getEquipments() {
    return this.equipments;
  }
  public void setEquipments(Set<Equipment> equipments) {
    this.equipments = equipments;
  }
  @ManyToOne
  @JoinColumn(name="contract_id")
  public Contract getContract()
  {
    return this.contract;
  }
  public void setContract(Contract contract) {
    this.contract = contract;
  }
  @Column(name="DETAIL_INSTALL_SITE", nullable=true, length=2000)
  public String getDetailInstallSite() {
    return this.detailInstallSite;
  }
  public void setDetailInstallSite(String detailInstallSite) {
    this.detailInstallSite = detailInstallSite;
  }
  @Column(name="APPROVAL_SCRAP_DATE", nullable=true)
  public Date getApprovalScrapDate() {
    return this.approvalScrapDate;
  }
  public void setApprovalScrapDate(Date approvalScrapDate) {
    this.approvalScrapDate = approvalScrapDate;
  }
  @Column(name="ASSET_PIC", nullable=true, length=400)
  public String getAssetPic() {
    return this.assetPic;
  }
  public void setAssetPic(String assetPic) {
    this.assetPic = assetPic;
  }
  @Column(name="OVERHAUL_RATE", nullable=true, length=400)
  public String getOverhaulRate() {
    return this.overhaulRate;
  }
  public void setOverhaulRate(String overhaulRate) {
    this.overhaulRate = overhaulRate;
  }
  @Column(name="COMPLETE_TRANS_ASSET_TYPE", nullable=true, length=400)
  public String getCompleteTransAssetType() {
    return this.completeTransAssetType;
  }
  public void setCompleteTransAssetType(String completeTransAssetType) {
    this.completeTransAssetType = completeTransAssetType;
  }
  @Column(name="OPERATE_PROJECT_ASSET", nullable=true, length=400)
  public String getOperateProjectAsset() {
    return this.operateProjectAsset;
  }
  public void setOperateProjectAsset(String operateProjectAsset) {
    this.operateProjectAsset = operateProjectAsset;
  }
  @Column(name="OPERATE_PROJECT_ASSET_DATE", nullable=true)
  public Date getOperateProjectAssetDate() {
    return this.operateProjectAssetDate;
  }
  public void setOperateProjectAssetDate(Date operateProjectAssetDate) {
    this.operateProjectAssetDate = operateProjectAssetDate;
  }
  @Column(name="PROJECT_APP_DOC_NO", nullable=true, length=400)
  public String getProjectAppDocNo() {
    return this.projectAppDocNo;
  }
  public void setProjectAppDocNo(String projectAppDocNo) {
    this.projectAppDocNo = projectAppDocNo;
  }
  @Column(name="PROJECT_NO", nullable=true, length=2000)
  public String getProjectNo() {
    return this.projectNo;
  }
  public void setProjectNo(String projectNo) {
    this.projectNo = projectNo;
  }
  @Column(name="CONTRACT", nullable=true, length=2000)
  public String getContractNo() {
    return this.contractNo;
  }
  public void setContractNo(String contractNo) {
    this.contractNo = contractNo;
  }
  @Column(name="DATA_LIST", nullable=true, length=100)
  public String getDataList() {
    return this.dataList;
  }
  public void setDataList(String dataList) {
    this.dataList = dataList;
  }
  @Column(name="EQUIPMENT_LIST", nullable=true, length=100)
  public String getEquipmentList() { return this.equipmentList; }
  public void setEquipmentList(String equipmentList) {
    this.equipmentList = equipmentList;
  }
  @Column(name="OVERHAUL_FINAL_PRICE", nullable=true, length=100)
  public Double getOverhaulFinalPrice() { return this.overhaulFinalPrice; }
  public void setOverhaulFinalPrice(Double overhaulFinalPrice) {
    this.overhaulFinalPrice = overhaulFinalPrice;
  }
  @Column(name="FACTORY_PRICE", nullable=true)
  public Double getFactoryPrice() {
    return this.factoryPrice;
  }
  public void setFactoryPrice(Double factoryPrice) {
    this.factoryPrice = factoryPrice;
  }
  @Column(name="CONTRACT_PRICE", nullable=true)
  public Double getContractPrice() {
    return this.contractPrice;
  }
  public void setContractPrice(Double contractPrice) {
    this.contractPrice = contractPrice;
  }
  @Column(name="ORIGINAL_VALUE", nullable=true)
  public Double getOriginalValue() {
    return this.originalValue;
  }
  public void setOriginalValue(Double originalValue) {
    this.originalValue = originalValue;
  }
  @Column(name="DEPRECIATION_MeTHOD", nullable=true)
  public String getDepreciationMethod() {
    return this.depreciationMethod;
  }
  public void setDepreciationMethod(String depreciationMethod) {
    this.depreciationMethod = depreciationMethod;
  }
}
```
```java
package com.wonders.asset.model;
import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
@Entity
@Table(name="T_EQUIPMENTS")
public class Equipment extends SystemInformation
{
  private String name;
  private String type;
  private String manufactureCountry;
  private Date useDate;
  private Double serviceLife;
  private String Location;
  private Double price;
  private Enterprise manufacturer;
  private Enterprise supplier;
  private Asset asset;
  @Column(name="NAME", nullable=true, length=400)
  public String getName()
  {
    return this.name;
  }
  public void setName(String name) {
    this.name = name;
  }
  @Column(name="TYPE", nullable=true, length=400)
  public String getType() {
    return this.type;
  }
  public void setType(String type) {
    this.type = type;
  }
  @Column(name="MANUFACTURE_COUNTRY", nullable=true, length=400)
  public String getManufactureCountry() {
    return this.manufactureCountry;
  }
  public void setManufactureCountry(String manufactureCountry) {
    this.manufactureCountry = manufactureCountry;
  }
  @Column(name="USE_DATE", nullable=true)
  public Date getUseDate() {
    return this.useDate;
  }
  public void setUseDate(Date useDate) {
    this.useDate = useDate;
  }
  @Column(name="SERVICE_LIFE", nullable=true)
  public Double getServiceLife() {
    return this.serviceLife;
  }
  public void setServiceLife(Double serviceLife) {
    this.serviceLife = serviceLife;
  }
  @Column(name="LOCATION", nullable=true, length=400)
  public String getLocation() {
    return this.Location;
  }
  public void setLocation(String location) {
    this.Location = location;
  }
  @Column(name="PRICE", nullable=true)
  public Double getPrice() {
    return this.price;
  }
  public void setPrice(Double price)
  {
    this.price = price;
  }
  @ManyToOne
  @JoinColumn(name="manufacturer_id")
  public Enterprise getManufacturer() { return this.manufacturer; }
  public void setManufacturer(Enterprise manufacturer) {
    this.manufacturer = manufacturer;
  }
  @ManyToOne
  @JoinColumn(name="supplier_id")
  public Enterprise getSupplier() { return this.supplier; }
  public void setSupplier(Enterprise supplier) {
    this.supplier = supplier;
  }
  @ManyToOne
  @JoinColumn(name="ASSET_ID")
  public Asset getAsset() { return this.asset; }
  public void setAsset(Asset asset) {
    this.asset = asset;
  }
}
```
```java
package com.wonders.asset.model;
import java.io.Serializable;
import java.util.Set;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import org.hibernate.annotations.GenericGenerator;
@Entity
@Table(name="T_CONTRACT")
public class Contract
  implements Serializable
{
  private String id;
  private String buildSupplierId;
  private String buildSupplierName;
  private String contractAttachment;
  private String contractContent;
  private String contractName;
  private String contractNo;
  private String contractOwnerId;
  private String contractOwnerName;
  private String contractPassedDate;
  private String contractPlanId;
  private String contractPlanNo;
  private String contractPrice;
  private String contractSignedDate;
  private String contractStatus;
  private String contractType;
  private String createDate;
  private String payType;
  private String projectId;
  private String projectName;
  private String projectNo;
  private String remark;
  private String removed;
  private String updateDate;
  private String contractEndDate;
  private String contractStartDate;
  private String contractOwnerExecuteId;
  private String contractOwnerExecuteName;
  private String verifyPrice;
  private String finalPrice;
  private String inviteBidType;
  private String nationVerifyPrice;
  private String selfNo;
  private String createType;
  private String contractDestoryDate;
  private String registerPersonName;
  private String registerPersonLoginname;
  private String contractGrouping;
  private Set<Asset> assets;
  @Column(name="CONTRACT_DESTORY_DATE", nullable=true, length=40)
  public String getContractDestoryDate()
  {
    return this.contractDestoryDate;
  }
  public void setContractDestoryDate(String contractDestoryDate) {
    this.contractDestoryDate = contractDestoryDate;
  }
  @Column(name="CREATE_TYPE", nullable=true, length=200)
  public String getCreateType() {
    return this.createType;
  }
  public void setCreateType(String createType) {
    this.createType = createType;
  }
  @Column(name="SELF_NO", nullable=true, length=200)
  public String getSelfNo() {
    return this.selfNo;
  }
  public void setSelfNo(String selfNo) {
    this.selfNo = selfNo;
  }
  public void setId(String id) {
    this.id = id; } 
  @Id
  @GeneratedValue(generator="system.uuid")
  @GenericGenerator(name="system.uuid", strategy="uuid")
  @Column(name="ID")
  public String getId() { return this.id; }
  public void setBuildSupplierId(String buildSupplierId)
  {
    this.buildSupplierId = buildSupplierId;
  }
  @Column(name="BUILD_SUPPLIER_ID", nullable=true, length=40)
  public String getBuildSupplierId() {
    return this.buildSupplierId;
  }
  public void setBuildSupplierName(String buildSupplierName) {
    this.buildSupplierName = buildSupplierName;
  }
  @Column(name="BUILD_SUPPLIER_NAME", nullable=true, length=40)
  public String getBuildSupplierName() {
    return this.buildSupplierName;
  }
  public void setContractAttachment(String contractAttachment) {
    this.contractAttachment = contractAttachment;
  }
  @Column(name="CONTRACT_ATTACHMENT", nullable=true, length=40)
  public String getContractAttachment() {
    return this.contractAttachment;
  }
  public void setContractContent(String contractContent) {
    this.contractContent = contractContent;
  }
  @Column(name="CONTRACT_CONTENT", nullable=true, length=4000)
  public String getContractContent() {
    return this.contractContent;
  }
  public void setContractEndDate(String contractEndDate) {
    this.contractEndDate = contractEndDate;
  }
  @Column(name="CONTRACT_END_DATE", nullable=true, length=40)
  public String getContractEndDate() {
    return this.contractEndDate;
  }
  public void setContractName(String contractName) {
    this.contractName = contractName;
  }
  @Column(name="CONTRACT_NAME", nullable=true, length=140)
  public String getContractName() {
    return this.contractName;
  }
  public void setContractNo(String contractNo) {
    this.contractNo = contractNo;
  }
  @Column(name="CONTRACT_NO", nullable=true, length=100)
  public String getContractNo() {
    return this.contractNo;
  }
  public void setContractOwnerId(String contractOwnerId) {
    this.contractOwnerId = contractOwnerId;
  }
  @Column(name="CONTRACT_OWNER_ID", nullable=true, length=40)
  public String getContractOwnerId() {
    return this.contractOwnerId;
  }
  public void setContractOwnerName(String contractOwnerName) {
    this.contractOwnerName = contractOwnerName;
  }
  @Column(name="CONTRACT_OWNER_NAME", nullable=true, length=1000)
  public String getContractOwnerName() {
    return this.contractOwnerName;
  }
  public void setContractPassedDate(String contractPassedDate) {
    this.contractPassedDate = contractPassedDate;
  }
  @Column(name="CONTRACT_PASSED_DATE", nullable=true, length=40)
  public String getContractPassedDate() {
    return this.contractPassedDate;
  }
  public void setContractPlanId(String contractPlanId) {
    this.contractPlanId = contractPlanId;
  }
  @Column(name="CONTRACT_PLAN_ID", nullable=true, length=40)
  public String getContractPlanId() {
    return this.contractPlanId;
  }
  public void setContractPlanNo(String contractPlanNo) {
    this.contractPlanNo = contractPlanNo;
  }
  @Column(name="CONTRACT_PLAN_NO", nullable=true, length=40)
  public String getContractPlanNo() {
    return this.contractPlanNo;
  }
  public void setContractPrice(String contractPrice) {
    this.contractPrice = contractPrice;
  }
  @Column(name="CONTRACT_PRICE", nullable=true, length=40)
  public String getContractPrice() {
    return this.contractPrice;
  }
  public void setContractSignedDate(String contractSignedDate) {
    this.contractSignedDate = contractSignedDate;
  }
  @Column(name="CONTRACT_SIGNED_DATE", nullable=true, length=40)
  public String getContractSignedDate() {
    return this.contractSignedDate;
  }
  public void setContractStatus(String contractStatus) {
    this.contractStatus = contractStatus;
  }
  @Column(name="CONTRACT_STATUS", nullable=true, length=1)
  public String getContractStatus() {
    return this.contractStatus;
  }
  public void setContractType(String contractType) {
    this.contractType = contractType;
  }
  @Column(name="CONTRACT_TYPE", nullable=true, length=40)
  public String getContractType() {
    return this.contractType;
  }
  public void setCreateDate(String createDate) {
    this.createDate = createDate;
  }
  @Column(name="CREATE_DATE", nullable=true, length=40)
  public String getCreateDate() {
    return this.createDate;
  }
  public void setPayType(String payType) {
    this.payType = payType;
  }
  @Column(name="PAY_TYPE", nullable=true, length=400)
  public String getPayType() {
    return this.payType;
  }
  public void setProjectId(String projectId) {
    this.projectId = projectId;
  }
  @Column(name="PROJECT_ID", nullable=true, length=100)
  public String getProjectId() {
    return this.projectId;
  }
  public void setProjectName(String projectName) {
    this.projectName = projectName;
  }
  @Column(name="PROJECT_NAME", nullable=true, length=200)
  public String getProjectName() {
    return this.projectName;
  }
  public void setProjectNo(String projectNo) {
    this.projectNo = projectNo;
  }
  @Column(name="PROJECT_NO", nullable=true, length=40)
  public String getProjectNo() {
    return this.projectNo;
  }
  public void setRemark(String remark) {
    this.remark = remark;
  }
  @Column(name="REMARK", nullable=true, length=4000)
  public String getRemark() {
    return this.remark;
  }
  public void setRemoved(String removed) {
    this.removed = removed;
  }
  @Column(name="REMOVED", nullable=true, length=1)
  public String getRemoved() {
    return this.removed;
  }
  public void setUpdateDate(String updateDate) {
    this.updateDate = updateDate;
  }
  @Column(name="UPDATE_DATE", nullable=true, length=40)
  public String getUpdateDate() {
    return this.updateDate;
  }
  @Column(name="CONTRACT_START_DATE", nullable=true, length=40)
  public String getContractStartDate() {
    return this.contractStartDate;
  }
  public void setContractStartDate(String contractStartDate) {
    this.contractStartDate = contractStartDate;
  }
  @Column(name="CONTRACT_OWNER_EXECUTE_ID", nullable=true, length=40)
  public String getContractOwnerExecuteId() {
    return this.contractOwnerExecuteId;
  }
  public void setContractOwnerExecuteId(String contractOwnerExecuteId) {
    this.contractOwnerExecuteId = contractOwnerExecuteId;
  }
  @Column(name="CONTRACT_OWNER_EXECUTE_NAME", nullable=true, length=40)
  public String getContractOwnerExecuteName() {
    return this.contractOwnerExecuteName;
  }
  public void setContractOwnerExecuteName(String contractOwnerExecuteName) {
    this.contractOwnerExecuteName = contractOwnerExecuteName;
  }
  @Column(name="VERIFY_PRICE", nullable=true, length=100)
  public String getVerifyPrice() {
    return this.verifyPrice;
  }
  public void setVerifyPrice(String verifyPrice) {
    this.verifyPrice = verifyPrice;
  }
  @Column(name="FINAL_PRICE", nullable=true, length=100)
  public String getFinalPrice() {
    return this.finalPrice;
  }
  public void setFinalPrice(String finalPrice) {
    this.finalPrice = finalPrice;
  }
  @Column(name="INVITE_BID_TYPE", nullable=true, length=20)
  public String getInviteBidType() {
    return this.inviteBidType;
  }
  public void setInviteBidType(String inviteBidType) {
    this.inviteBidType = inviteBidType;
  }
  @Column(name="NATION_VERIFY_PRICE", nullable=true, length=100)
  public String getNationVerifyPrice() {
    return this.nationVerifyPrice;
  }
  public void setNationVerifyPrice(String nationVerifyPrice) {
    this.nationVerifyPrice = nationVerifyPrice;
  }
  @Column(name="REGISTER_PERSON_NAME", nullable=true, length=200)
  public String getRegisterPersonName() {
    return this.registerPersonName;
  }
  public void setRegisterPersonName(String registerPersonName) {
    this.registerPersonName = registerPersonName;
  }
  @Column(name="REGISTER_PERSON_LOGINNAME", nullable=true, length=100)
  public String getRegisterPersonLoginname() {
    return this.registerPersonLoginname;
  }
  public void setRegisterPersonLoginname(String registerPersonLoginname) {
    this.registerPersonLoginname = registerPersonLoginname;
  }
  @Column(name="CONTRACT_GROUPING", nullable=true, length=1)
  public String getContractGrouping() {
    return this.contractGrouping;
  }
  public void setContractGrouping(String contractGrouping) {
    this.contractGrouping = contractGrouping;
  }
  @OneToMany(cascade={javax.persistence.CascadeType.ALL}, fetch=FetchType.LAZY, mappedBy="contract")
  public Set<Asset> getAssets() {
    return this.assets;
  }
  public void setAssets(Set<Asset> assets) {
    this.assets = assets;
  }
}
```
