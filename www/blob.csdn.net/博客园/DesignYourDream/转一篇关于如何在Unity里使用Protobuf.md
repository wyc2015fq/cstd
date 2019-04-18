# 转一篇关于如何在Unity里使用Protobuf - DesignYourDream - 博客园
# [转一篇关于如何在Unity里使用Protobuf](https://www.cnblogs.com/designyourdream/p/4268092.html)
原帖地址：
http://purdyjotut.blogspot.com/2013/10/using-protobuf-in-unity3d.html
先转过来，等时间合适了，再来收拾
### Using Protobuf In Unity3D
Recently in our projects at LoneWolfPack Interactive, we looked for faster and more efficient ways to store data. In the past we had fiddled with XML and .Net's binary formatter, but so far we weren't very satisfied.
XML is  nice. You can store practically any data, and then edit it afterwards once the file is stored. The problem with it is that the readability of the files can make it extremely bloated with certain data sets.. You could go from small manageable files to massive uncontrollable files depending on the amount of data you need to store. The speed of serialization and deserialization in the XMLSerializer is also fairly slow, so you can get small chugs here and there when you need to use it.
Binary formatter was the temporary solution. It stored small files and was relatively quick. There are a few problems with the binary formatter though, such as the lack of flexibility. If you save data using the binary formatter and then change the saved class, you come across issues and you have to scrap the data that was there. The other problem is that binary formatter isn't supported on iOS. Binary formatter requires the ability to JIT compile (Just In Time), where Apple requires AOT compilation (Ahead Of Time).
Looking for another solution I hit the net, and came across protobuf-net which was developed by Marc Gravell of Stack Overflow. If you are a geek and like stats you can look up the performance info [here](https://code.google.com/p/protobuf-net/wiki/Performance). Basically protobuf solved all of the issues that we were having with binary formatter. It was even faster than binary formatter, it was flexible, it was supported on iOS and we could change around our classes all we wanted and it wouldn't matter, things would still load.
There is a little bit of set up with protobuf though, and for that we are going to use Visual Studio. The only reason I am using Visual Studio here instead of MonoDevelop is because VS handles a few things automatically for us and the whole process is quicker. Plus I like Visual Studio, and I miss using it. If you don't have Visual Studio, you can grab it [here](http://www.microsoft.com/visualstudio/eng/products/visual-studio-2010-express). In this tutorial I am using VS 2010, but everything should be similar if you grab [VS express 2012](http://www.microsoft.com/visualstudio/eng/downloads#d-2012-express) .
The first thing you want to do is download protobuf from [Marc's Google Code site](https://code.google.com/p/protobuf-net/downloads/list). As of the date of this post, the version is r668, but unless Marc goes crazy and restructures everything, future versions should work with this tutorial as well.
Once the zip file containing protobuf is downloaded, you are going to want to grab the protobuf-net.dll file located under Full/unity/. and put it somewhere where you will remember so that you can reference it in future projects.
![](https://images0.cnblogs.com/blog/698097/201502/021623087187633.jpg)
Once we have the dll stored on our computer in an accessible place, let's open Visual Studio, and create a brand new Class Library project. This project is used for creating a dll, and this will allow us to access the classes and structures we want to save within our Unity project. So since we are saving game data, let's call this project GameSaveData.
![](https://images0.cnblogs.com/blog/698097/201502/021624333743697.jpg)
Since we are using this in Unity, we want to set a few things up beforehand. The first thing we want to do is make sure that the project is compiled under .Net Framework 2. In order to do this, in the menu go to Project -> GameSaveData properties, and set the target framework to 2.0. You will get a warning saying that Visual Studio needs to reopen to make the changes. Hit yes and you can continue on.
![](https://images0.cnblogs.com/blog/698097/201502/021625090782338.jpg)
Next we want to reference protobuf in our project. In order to do this, right click on your references folder in the Solution Explorer and hit Add Reference. From here you can browse to where you stored the protobuf and select it.
![](https://images0.cnblogs.com/blog/698097/201502/021625398598673.jpg)
Now that our project is set up, here is a sample version of what your GameSaveData might look like. This sample uses a basic item class, a few enums and a character customization class:
using System;
//we need to use specific attributes so that ProtoBuf knows what references what, so we add a using statement for the ProtoBuf namespace.
using ProtoBuf;
namespace GameSaveData
{
//when creating a class use the ProtoContract attribute. While not completely necessary if you look through the documentation you can see that these attributes can control what goes on when things are serialized and deserialized
    [ProtoContract]
    public class Item
    {
//Protobuf also needs to associate data with keys so that it can identify data on serialization and deserialization. We do this with the ProtoMember attribute, and in that attribute be assign an integer value
        [ProtoMember(1)]
        public string name;
        [ProtoMember(2)]
        public int price;
        [ProtoMember(3)]
        public EntityType applicableEntities;
        [ProtoMember(4)]
        public ItemType itemType;
//then we can just make the class as normal
        public Item()
        {
            this.name = "Item";
            this.price = 10;
            this.applicableEntities = EntityType.ALL;
            this.itemType = ItemType.Hat;
        }
        public Item(string name, int price,EntityType applicableEntities, ItemType currentItemType)
        {
            this.name = name;
            this.price = price;
            this.applicableEntities = applicableEntities;
            this.itemType = currentItemType;
        }
    }
    [ProtoContract]
    public enum EntityType
    {
        NONE = 0,
        ROCK = 1,
        PAPER = 1 << 1,
        SCISSORS = 1 << 2,
        NEUTRAL = 1 << 3,
        ALL = ROCK | PAPER | SCISSORS | NEUTRAL
    }
    [ProtoContract]
    public enum ItemType
    {
        Hat = 0,
        Cape = 1,
        Weapon = 2,
        Shoe = 3,
        Arm_Armour = 4,
        Leg_Armour = 5,
        Shirt = 6,
    }
    [ProtoContract]
    public class CharacterCustomizationData
    {
        [ProtoMember(1)]
        public Item hat;
        [ProtoMember(2)]
        public Item cape;
        [ProtoMember(3)]
        public Item shoes;
        [ProtoMember(4)]
        public Item weapon;
        [ProtoMember(5)]
        public Item armArmour;
        [ProtoMember(6)]
        public Item legArmour;
        [ProtoMember(7)]
        public Item shirt;
        public CharacterCustomizationData()
        {
            this.hat = this.cape = this.shoes = this.weapon = this.armArmour = this.legArmour = this.shirt = null;
        }
    }
//just a very basic trophy implementation for those who would put trophies in their game and need a way to store the data
    [ProtoContract]
    public class Trophy
    {
        [ProtoMember(1)]
        public string trophyName;
        [ProtoMember(2)]
        public string trophyDescription;
        public Trophy()
        {
            trophyName = "Trophy1";
            trophyDescription = "Description";
        }
        public Trophy(string trophyName, string trophyDescription)
        {
            this.trophyName = trophyName;
            this.trophyDescription = trophyDescription;
        }
    }
}
From here, just build the project and you are done with GameSaveData.
Now, if you are deploying on a platform that handles JIT compilation, this is all you need. From here you can go to the bin folder in your GameSaveData project and drag the GameSaveData.dll and Protobut-net.dll into your Unity Project under the plugins folder.
![](https://images0.cnblogs.com/blog/698097/201502/021627248127737.jpg)
From here, using Protobuf in your project is really simple. If you are trying to read data from a while:
using (FileStream f = new FileStream("testpath", FileMode.OpenOrCreate))
{
     mySerializedObject = ProtoBuf.Serializer.Deserialize<ObjectType>(f);
}
and if you are saving to a file:
using (FileStream f = new FileStream("testpath", FileMode.OpenOrCreate))
{
    ProtoBuf.Serializer.Serialize<ObjectType>(f, mySerializedObject);
}
Now, if you are deploying to a platform that does not support JIT, you have a little bit more work to do. In order for you do use protobuf in an AOT environment, you need to pre-compile the serializer that will be handling all of your data. Thankfully this is a very simple process, and takes about the same time to complete as our first Visual Studio project.
So what we want to do is create another new Visual Studio project, and this time it is going to be a console application. We will want to call this SaveDataSerializer, since that is what we are serializing.
![](https://images0.cnblogs.com/blog/698097/201502/021627544531561.jpg)
We want to set the project up very similarly to our first one, so once again we are going to change the target framework to .Net 2.0 in  Project -> SaveDataSerializer properties.
We also want to add references to the project again, but this time, we want to add references to the GameSaveData.dll and protobuf-net.dll that were compiled in our last project. They should be in the bin/Debug or bin/Release folder, depending on the settings that you had the project in.
![](https://images0.cnblogs.com/blog/698097/201502/021628282819427.jpg)
Now that our project is set up, we can create our program that will compile our serializer for us. Using the sample, our serializer code would look something like this:
using System;
//we need to reference the content of our old project so that we can add the data to the serializer
using GameSaveData;
//we need to reference the ProtoBuf.Meta library so that we can create a TypeModel that will tell protobuf exactly how the data is serialized
using ProtoBuf.Meta;
namespace GameDataSerializer
{
    class Program
    {
        static void Main(string[] args)
        {
            var model = TypeModel.Create();
            model.Add(typeof(EntityType), true);
            model.Add(typeof(ItemType), true);
            model.Add(typeof(Item), true);
            model.Add(typeof(CharacterCustomizationData), true);
            model.Add(typeof(Trophy), true);
            model.AllowParseableTypes = true;
            model.AutoAddMissingTypes = true;
            model.Compile("GameDataSerializer", "GameDataSerializer.dll");
        }
    }
}
From here, you can use Protobuf in any of your projects, whether it is iOS, Android, PC, whichever. If you need to modify your classes, just open up your Visual Studio projects, make the modifications and go through the tutorial again. That is the nice thing about Protobuf, is that it is so easy to manipulate and change, and all of your data is safe.
Now normally I would say that I am done, but there is one more thing I want to show you, and that is a wrapper class that I created for protobuf. This class allows you to use protobuf anywhere in your project, is much more manageable, and much more convenient to use. So what I want you to do is create a new c# script in your Unity project, and call it ProtoLoader.
using UnityEngine;
using System.IO;
//this is a static class. We create this because we dont need an instance of it to make anything work, everything works independently.
public static class ProtoLoader
{
//We need to make a reference to the GameDataSerializer that wil be handling all of the data
    private static GameDataSerializer m_serializer = new GameDataSerializer();
//our first function will load an object from resources. This is useful for if you had an editor script that created data that would be stored for later usage in the game, and you had saved it with the .bytes extension
    public static T loadObjectFromResources<T>(string resourcePath)
    {
        TextAsset objectAsset = Resources.Load(resourcePath, typeof(TextAsset)) as TextAsset;
        if(objectAsset == null)
        {
            return default(T);
        }
        T deserializedObject = default(T);
//basically we just load the bytes of the text asset into a memory stream, and the deserialize it from there
        using(MemoryStream m = new MemoryStream(objectAsset.bytes))
        {
            deserializedObject = (T)m_serializer.Deserialize(m, null, typeof(T));
        }
        return deserializedObject;
    }
//our next function is used for loading an object from a path. Basically for when you are loading progress and other sorts of stuffs
    public static T loadObjectFromPath<T>(string path)
    {
//if the file doesn't exist we just return null. Since we are using a templated function the best we can do is default(T)
        if(!File.Exists(path))
        {
            return default(T);
        }
        T deserializedObject = default(T);
        using(FileStream f = new FileStream(path, FileMode.Open))
        {
            deserializedObject = (T)m_serializer.Deserialize(f, null, typeof(T));
        }
        return deserializedObject;
    }
//Our third function will save the object to a file path. This is basically for saving game options, progress, whatever you can think of that you would need saved
    public static void saveObjectToPath<T>(string objPath, string fileName, T serializedObject)
    {
        if(!Directory.Exists(objPath))
        {
            Directory.CreateDirectory(objPath);
        }
//after checking if the directory is there we serialize the object into a filestream to save it.
        using(FileStream f = new FileStream(objPath + fileName, FileMode.OpenOrCreate))
        {
            m_serializer.Serialize(f, serializedObject);
        }
    }
//This function is actually useful for sending objects over an RPC function. Unity allows you to send a byte[]  through an RPC even though its not documented, so here you can serialize object data, send it over the network and then deserialize the object on the other end.
    public static byte[] serializeProtoObject<T>(T obj)
    {
        using(MemoryStream m = new MemoryStream())
        {
            m_serializer.Serialize(m, obj);
            return m.ToArray();
        }
    }
//this is the opposite of the previous function. It allows you to send in a byte[] and return the object that it originally was. Very useful for networking
    public static T deserializeProtoObject<T>(byte[] bytes)
    {
        using(MemoryStream m = new MemoryStream(bytes))
        {
            return (T)m_serializer.Deserialize(m, null, typeof(T));
        }
    }
}
And that is it! You now have fast, efficient and easy to use serialization for your project!
Overall we are really happy with how well protobuf works, and it is definitely something for you to look into for data serialization. Now for those of you who may have had an issue here or there, or just want the projects, I have included a[link here](https://db.tt/3ldlLb03) to a zip file that has both of the visual studio projects, and a unity package that has all of the classes and dlls created in this project, at your convenience, because I am super nice and awesome like that. [LINK](https://db.tt/3ldlLb03)
Remember to share if you liked it, whether its on Twitter, Facebook, Google+ or to your technically savvy grandma!

