# 在Ceph创建虚拟机的过程改进分析 - weixin_33985507的博客 - CSDN博客
2015年08月19日 16:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5

# 作为个人学习笔记分享。有不论什么问题欢迎交流！
近期在Gerrit中看到一个change：[https://review.openstack.org/#/c/94295/](https://review.openstack.org/#/c/94295/) ， 它主要是对当前在Ceph中创建虚拟机的流程的改进。假设glance的backend是ceph, 则nova创建虚拟机到RBD的流程是这种：
通过glance从ceph中下载image --> 本地 --> 复制image到rbd
这个change的目的就是：不须要下载到本地。直接在rbd中复制image,以提高虚拟机创建的速度。
曾经仅仅知道nova从glance下载image,作为虚拟机的base,却没有细致的了解过这个过程，正好借这个机会。看一看nova创建虚拟机到rbd过程中关于image的部分。
## 1 nova创建VM时image的流程
 经过nova-scheduler选择节点后，创建VM的请求到达了nova-compute。即nova/compute/manager.py：ComputeManager._run_instance()：
```python
def _run_instance(self, context, request_spec,
                      filter_properties, requested_networks, injected_files,
                      admin_password, is_first_time, node, instance,
                      legacy_bdm_in_spec):
```
关于镜像的元数据就保存在request_spec。
```python
image_meta = request_spec['image']
```
取得元数据后就開始build_instance()了。可是以下的过程与image没太大关系，所以从简带过。
```python
def _build_instance(self, context, request_spec, filter_properties,
            requested_networks, injected_files, admin_password,   is_first_time,
node, instance, image_meta, legacy_bdm_in_spec)
---->
def _spawn(self, context, instance, image_meta, network_info,
               block_device_info, injected_files, admin_password,
               set_access_ip=False)
---->
 
self.driver.spawn(context, instance, image_meta,
                              injected_files, admin_password,
                              network_info,
                              block_device_info)
```
这里的driver就是你用的Hypervioser, 我用的是KVM，所以这个driver.spawn=nova/virt/libvirt/driver.py:LibvirtDriver.spawn()：
```python
def spawn(self, context, instance, image_meta, injected_files,
              admin_password,network_info=None,  block_device_info=None)：
       disk_info = blockinfo.get_disk_info(CONF.libvirt.virt_type,
                                            instance,
                                            block_device_info,
                                            image_meta)
        self._create_image(context, instance,
                           disk_info['mapping'],
                           network_info=network_info,
                           block_device_info=block_device_info,
                           files=injected_files,
                           admin_pass=admin_password)
```
那么这个disk_info['mapping']是什么呢？这里有一个方法，我们能够从test_libvirt_blockinfo.py里找到答案，所以结合測试用例来看代码真的非常实用。在Nova/tests/virt/libvirt/test_libvirt_blockinfo.py:
LibvirtBlockInfoTest.test_get_disk_mapping_simple_swap()里能够看到：
```python
expect = {
            'disk': {'bus': 'virtio', 'dev': 'vda',
                     'type': 'disk', 'boot_index': '1'},
            'disk.local': {'bus': 'virtio', 'dev': 'vdb', 'type': 'disk'},
            'root': {'bus': 'virtio', 'dev': 'vda',
                     'type': 'disk', 'boot_index': '1'}
            }
```
Expect就是期望disk_info['mapping']的样子。
以下就開始创建VM的image了：
```python
def _create_image(self, context, instance,
                      disk_mapping, suffix='',
                      disk_images=None, network_info=None,
                      block_device_info=None, files=None,
                      admin_pass=None, inject_files=True):
#以下这个函数就是返回VM image格式的相关类，位于 #libvirt/imagebackend.py中，这里image是rbd, 返回的就是Rbd    #类。
```
def image(fname, image_type=CONF.libvirt.images_type):
            return self.image_backend.image(instance,
                                     fname + suffix, image_type)
......
 
  if not booted_from_volume:
            root_fname = imagecache.get_cache_fname(disk_images, 'image_id')#以image id作为文件名称
            size = instance['root_gb'] * units.Gi
 
            if size == 0 or suffix == '.rescue':
                size = None
 
#这里有点复杂，用到了回调函数fetch_image，这里的cache是Rbd的父类#Image类的cache（），主要功能是从模板，也就是glance的image。 为VM创建一个image.
           image('disk').cache(fetch_func=libvirt_utils.fetch_image,
                                context=context,
                                filename=root_fname,
                                size=size,
                                image_id=disk_images['image_id'],
                                user_id=instance['user_id'],
                               project_id=instance['project_id'])
#能够在cache()中看到：
        if not self.check_image_exists() or not os.path.exists(base):
            self.create_image(fetch_func_sync, base, size,
                              *args, **kwargs)
那么如今到class Rbd下能够找到create_image():
```python
def create_image(self, prepare_template, base, size, *args, **kwargs):
        if self.rbd is None:
            raise RuntimeError(_('rbd python libraries not found'))
 
        if not os.path.exists(base):
            prepare_template(target=base, max_size=size, *args, **kwargs)##这里的prepare_temple()就是 libvirt_utils.fetch_image啦。
```
libvirt_utils.fetch_image=nova/virt/libvirt/utils.fetch_image()：
```python
def fetch_image(context, target, image_id, user_id, project_id, max_size=0):
    """Grab image."""
    images.fetch_to_raw(context, image_id, target, user_id, project_id,
                        max_size=max_size)
--->
 
def fetch_to_raw(context, image_href, path, user_id, project_id, max_size=0):
    path_tmp = "%s.part" % path
    fetch(context, image_href, path_tmp, user_id, project_id,
          max_size=max_size)
 
---->
def fetch(context, image_href, path, _user_id, _project_id, max_size=0):
(image_service, image_id) = glance.get_remote_image_service(
context, image_href)#从glance获取image
with fileutils.remove_path_on_error(path):
        #这里就是把image_id的数据download到path了。
```
Download()位于   #nova/image/glance.py。
        image_service.download(context, image_id, dst_path=path)
回到class Rbd的create_image()中。
libvirt_utils.import_rbd_image(*args)把path的image数据写入rbd,至此，整个流程就到这里结束了。
## 2 Change中的改进
如今回到文章開始中提到的那个change, 看看它是怎么实现的。
首先它在fetch_to_raw中增加了一个推断。
```python
def fetch_to_raw(context, image_href, path, user_id, project_id,  max_size=0):
#推断backend是否具有‘direct_fetch’的属性，假设有，则直接返回#direct_fetch（）
    if backend and hasattr(backend, 'direct_fetch'):
        try:
            return backend.direct_fetch(context, image_href)
        except exception.ImageUnacceptable:
            LOG.debug(_('could not fetch directly, falling back to download'))
```
给Rbd类加入了一个属性：
```python
def direct_fetch(self, context, image_href):
#推断driver是否支持layering（分层
#http://ceph.com/docs/firefly/dev/rbd-layering/ ，指的是块设备
#的cow克隆。支持高速创建image）
        if not self.driver.supports_layering():
            reason = _('installed version of librbd does not support cloning')
            raise exception.ImageUnacceptable(image_id=image_href,
                                              reason=reason)
 
        image_meta, locations = images.get_meta(context, image_href)
        LOG.debug(_('Image locations are: %(locs)s') % {'locs': locations})
 
        if image_meta.get('disk_format') not in ['raw', 'iso']:
            reason = _('Image is not raw format')
            raise exception.ImageUnacceptable(image_id=image_href,
                                              reason=reason)
#克隆镜像（http://ceph.com/docs/master/rbd/librbdpy/）
        for location in locations:
            if self.driver.is_cloneable(location, image_meta):
                return self.driver.clone(location, self.rbd_name)
 
        reason = _('No image locations are accessible')
        raise exception.ImageUnacceptable(image_id=image_href, reason=reason)
```
这样就不须要想1中的那样先把image下载到local, 在写到rbd中。直接在rbd中克隆，从而提高了虚拟机的创建速度。
## 3总结
借这个机会既熟悉了创建VM时的image流程。又熟悉了ceph的使用方法国，同时学习大师们是如何实现的功能，看起来review是啊大大受益。:)
版权声明：本文博客原创文章，博客，未经同意，不得转载。
