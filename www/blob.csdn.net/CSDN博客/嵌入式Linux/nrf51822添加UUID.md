
# nrf51822添加UUID - 嵌入式Linux - CSDN博客

2016年03月02日 18:07:10[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：3077


## 问题 ：添加一个自己的uuid
I’m new to use nordic and I would like to add a new custom service (2320EE58-8654-4132-95F7-0A872AC0958F) with a custom characteristic(23200000-8654-4132-95F7-0A872AC0958F) I’m using nRF51 and mBed compiler. Thank’s for a further reply and for the time. Regardes
## 添加uuid的步骤
Hi Epasta,
Nordic’s application note nAN-36 is the best guide for implementing custom services and characteristics.
Generally speaking, your service will take the form ABCDxxxx-EF12-3456-7890-ABCDEF123456 as a “base UUID” and then the 16-bits are used to identify the service itself and the characteristics under it. The lowercase x’s are where you specify your service/characteristc. For example, I implemented my service as f30900010-4903-4c71-8d16-549852aba4c5 and added a characteristic to it that had a full UUID as f3090011-4903-4c71-8d16-549852taba4c5 using the following code:
```python
#
```
```python
define
```
```python
PIP_UUID_BASE {0xC5, 0xA4, 0xAB, 0x52, 0x98, 0x54, 0x16, 0x8D, 0x71, 0x4C, 0x03, 0x4A, 0x00, 0x00, 0x09, 0xF3}
```
```python
#
```
```python
define
```
```python
LTES_UUID_SERVICE 0x0010
```
```python
#
```
```python
define
```
```python
LTES_UUID_DISCIP_CHAR 0x0011
```
From there, you have to add your register your base uuid with softdevice using:
```python
sd_ble_uuid_vs_add(ble_uuid128_t const
```
```python
*p_vs_uuid
```
```python
, uint8_t
```
```python
*p_uuid_type
```
```python
)
```
Then add your service using:
```python
sd_ble_gatts_service_add(uint8_t
```
```python
type
```
```python
,
```
```python
ble_uuid_t
```
```python
const
```
```python
*
```
```python
p_uuid
```
```python
,
```
```python
uint16_t
```
```python
*
```
```python
p_handle
```
```python
)
```
Then add your characteristic using:
```python
sd_ble_gatts_characteristic_add(uint16_t service_handle, ble_gatts_char_md_t const
```
```python
*p_char_md
```
```python
, ble_gatts_attr_t const
```
```python
*p_attr_char_value
```
```python
, ble_gatts_char_handles_t
```
```python
*p_handles
```
```python
)
```
Hope this helps!
Regards, michael

