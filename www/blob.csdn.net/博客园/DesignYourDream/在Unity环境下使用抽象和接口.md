# 在Unity环境下使用抽象和接口 - DesignYourDream - 博客园
# [在Unity环境下使用抽象和接口](https://www.cnblogs.com/designyourdream/p/4577219.html)
http://gamasutra.com/blogs/VictorBarcelo/20131217/207204/Using_abstractions_and_interfaces_with_Unity3D.php
Unity3D includes a **component architecture paradigm**. This allows us to attach code as classes that derive from MonoBehaviour to our GameObjects and treat them as script components.
With Unity3D GameObjects can communicate with scripts via `SendMessage` by just addressing the name of particular methods. We can also get the reference to the script with `GetComponent`. This brings us the safety of type checking but makes us rely on a concrete script reference which results in tight coupling (this can actually not be the case if the class inside the script inherits from some kind of abstraction, more on this later).
Besides the Unity3D component mojo we also have more classical approaches in the OO department that may suit our needs depending on the scenario.
![](http://victorbarcelo.net/wp-includes/js/tinymce/plugins/wordpress/img/trans.gif)One of them is to use an **observer pattern** based system, such as the one found in the **[Unity3D wiki](http://wiki.unity3d.com/index.php?title=CSharpMessenger_Extended)**, which is a fastest and cleaner alternative to Unity’s `SendMessage`. This solution is suitable if we wanted to communicate with a number of GameObjects without having to manage concrete references for each one.
We also have the alternative of using abstract references (abstract classes and interfaces). With these we can make our code less coupled and we can plug reusable logic without relying on concrete classes.
The following example shows how can you combine this approach with Unity's component based nature.
All the code and a working example can be found in this **[GitHub repository](https://github.com/Victor-Barcelo/Unity3D-OO-Techniques)**.
## Abstracting SHUMP entities
If you try to categorize the most used type of logic in a SHUMP you may agree with me if I point “movement patterns”. Any type of projectiles, enemies and power ups will move not always in a linear fashion. You may have diagonal, senoidal or saw tooth movement patterns and you are likely to combine them to make complex sequences that can for example fit a boss encounter.
Let’s start by creating a suitable interface for our movement algorithms.
public interface IMovement
{
    void Move(GameObject entity);
}
Now let’s show two movement patterns that implement this interface.
using UnityEngine;
public class LinearMovement : IMovement
{
    public void Move(GameObject go)
    {
        go.transform.Translate(Time.deltaTime * 10, 0f, 0f);
    }
}using UnityEngine;
public class SenoidalMovement : IMovement
{
    private const float amplitude = 1f;
    private const float frequency = 2f;
    public void Move(GameObject go)
    {
        float yMovement = amplitude * (Mathf.Sin(2 * Mathf.PI * frequency * Time.time) - Mathf.Sin(2 * Mathf.PI * frequency * (Time.time - Time.deltaTime)));
        go.transform.Translate(Time.deltaTime * 10f, yMovement, 0f);
    }
}
Neither the interface nor the movement logic need to be added to GameObjects since they don’t inherit from MonoBehaviour.
For this example our moving entity will be missiles, so let’s create a base class for them.
using UnityEngine;
public class BaseMissileBehaviour : MonoBehaviour
{
    private IMovement movementType;
    void Start()
    {
        Destroy(gameObject, 2f);
    }
    void Update()
    {
        movementType.Move(gameObject);
    }
    public void SetMovement(IMovement _movementType)
    {
        movementType = _movementType;
    }
}
This script has to be attached to a GameObject as a component. From here we can create a collection of prefabs to set up different missile visuals such as the model or particle systems.
Let's keep abstracting entities for the sake of reusability and create a weapon interface.
public interface IWeapon
{
    void Shoot();
}
Now let’s get concrete, I give you the laser cannon.
using UnityEngine;
public class LaserCannon : IWeapon
{
    private float fireDelay;
    private float timeSinceLastShoot;
    private GameObject owner;
    public GameObject projectilePrefab;
    public LaserCannon(float _fireDelay, GameObject _owner)
    {
        owner = _owner;
        fireDelay = _fireDelay;
    }
    public void Shoot()
    {
        if (Time.time > fireDelay + timeSinceLastShoot)
        {
            GameObject projectile =
                (GameObject)
                    GameObject.Instantiate(projectilePrefab, owner.transform.position, Quaternion.identity);
            projectile.GetComponent().SetMovement(new LinearMovement());
            timeSinceLastShoot = Time.time;
        }
    }
}
This way of structuring our code gives us the following benefits.
- We can reuse movement logic, just as we did in BaseMissileBehavior we can plug this logic onto any moving entity. This way of using interfaces is known as **strategy pattern**.
- We can encapsulate abstract entities (missiles) inside concrete entities (weapons). This let's us choose at which level we can assign attributes (like damage or fire delay in this example). In this case note that the overall configuration (which missile we use and which movement we plug inside them) takes place at the weapon level via its constructor.
- We can ask another class to give us instances of predefined weapons by supplying a name (preferably enum) or id. This class could query a data source (e.g. XML or SQLite) for the specific configuration of the given weapon. This is an application of the **factory pattern. **You can apply the same idea to entities like enemies (tip: if you are continuously creating entities with a low life span add to your factory **[object pooling](http://sourcemaking.com/design_patterns/object_pool)** to avoid performance penalty).
And finally this is how an entity could make use of a weapon via an interface.
using System.Collections.Generic;
using UnityEngine;
public class PlayerShip : MonoBehaviour
{
    private IWeapon activeWeapon;
    private List weapons;
    void Start()
    {
        weapons = new List { new MissileLauncher(0.5f, gameObject), new LaserCannon(0.5f, gameObject) };
    }
    public void Control()
    {        
        if (Input.GetKey(KeyCode.Space))
        {
            SetWeapon(weapons[0]);
            activeWeapon.Shoot();
        }
        if (Input.GetKey(KeyCode.RightControl))
        {
            SetWeapon(weapons[1]);
            activeWeapon.Shoot();
        }
    }
    private void SetWeapon(IWeapon _weapon)
    {
        activeWeapon = _weapon;
    }
}
This is a good example since you can see how easy it is to switch concrete classes based on its interface during runtime.
You can use this approach to create a movement pattern based of multiple movement behaviors as we discussed early. You just have to cycle through a collection of movement implementations every *x*time which is something easily doable with coroutines.
## GetComponent and abstractions
Actually `GetComponent` can fetch a script component by its superclass or interface implementation. I will show you an example of a scenario where you could use this, but first here you have some useful extension methods.
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
internal static class ExtensionMethods
{
    public static T GetAbstract(this GameObject inObj) where T : class
    {
        return inObj.GetComponents().OfType().FirstOrDefault();
    }
    public static T GetInterface(this GameObject inObj) where T : class
    {
        if (!typeof(T).IsInterface)
        {
            Debug.LogError(typeof(T).ToString() + ": is not an actual interface!");
            return null;
        }
        return inObj.GetComponents().OfType().FirstOrDefault();
    }
    public static IEnumerable GetInterfaces(this GameObject inObj) where T : class
    {
        if (!typeof(T).IsInterface)
        {
            Debug.LogError(typeof(T).ToString() + ": is not an actual interface!");
            return Enumerable.Empty();
        }
        return inObj.GetComponents().OfType();
    }
}
Let's picture a typical mouse driven action rpg. You may click on a door and you player will move to its proximity and the door will open. You could create a `ISOpenable` interface with an `Open` method. Since different doors may have a different opening logic (sliding, rotating etc) you could reference any type of door by its interface.
To make this work you could check for every mouse click if the mouse is hovering a GameObject that implements `ISOpenable` and call a `GetInterface().Open();`
You could also use this approach to attack enemies with something like`GetInterface().ApplyDamage(playerDamage);` and so on.
## Going further with abstractions
The art of plugging concrete classes on to abstractions is called **dependency injection**, and relies in the idea of** inversion of control** which is one of the core techniques of maintainable OO code.
If you want to go further in the practice of adding OO spice to your Unity3D development I recommend taking a look at **[StrangeIoC](http://thirdmotion.github.io/strangeioc/)** which is a framework that gives us a variety of tools, being one of them a cleaner mechanism to **[manage injections](http://martinfowler.com/articles/injection.html)**.

