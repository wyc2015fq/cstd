# DjangoRestFramework的外键反向关系序列化的一个问题 - =朝晖= - 博客园
# [DjangoRestFramework的外键反向关系序列化的一个问题](https://www.cnblogs.com/dhcn/p/7456988.html)
      先用文档中的样例：
      Models定义：
```
class Album(models.Model):
    album_name = models.CharField(max_length=100)
    artist = models.CharField(max_length=100)
class Track(models.Model):
    album = models.ForeignKey(Album, related_name='tracks', on_delete=models.CASCADE)
    order = models.IntegerField()
    title = models.CharField(max_length=100)
    duration = models.IntegerField()
    class Meta:
        unique_together = ('album', 'order')
        ordering = ['order']
    def __unicode__(self):
        return '%d: %s' % (self.order, self.title)
```
     关系序列化定义：
```
class TrackSerializer(serializers.ModelSerializer):
    class Meta:
        model = Track
        fields = ('order', 'title', 'duration')
class AlbumSerializer(serializers.ModelSerializer):
    tracks = TrackSerializer(many=True, read_only=True)
    class Meta:
        model = Album
        fields = ('album_name', 'artist', 'tracks')
```
      这个关系序列化有效的前提是必须在model定义中加
```
related_name='tracks'
```
     这个设置项，否则tracks在虚拟化结果中会找不到或者报错。

