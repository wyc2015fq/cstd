# MVC模式在UI里的应用 - DesignYourDream - 博客园
# [MVC模式在UI里的应用](https://www.cnblogs.com/designyourdream/p/4576754.html)
In a similar way to other parts of a game, user interfaces usually go through several iterations until we get the user experience right; it is vital that we can iterate over different ideas as fast as possible. Using a [MVC pattern](http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) to organize the view is a proven method to decouple how things are shown in the screen from the logic that controls how that view behaves upon interaction with the user, and how it should change depending on the application state. This organizational pattern provides several benefits:
- We can change how a UI widget looks like without modifying one single line of code
- We can share logic code in different controls and create complex views easily.
- Last but not least, we could change the underlying implementation of the view with relatively little effort. We are currently using the NGUI library to build our interfaces, but we are looking forward to work with the [upcoming UnityGUI](http://blogs.unity3d.com/2014/05/28/overview-of-the-new-ui-system/).
## Code Example
We’ve provided an [example Unity3D project available at our public git repository](https://github.com/socialpoint/engineering-blog-examples/blob/master/MVC/NGUI/Assets/Scripts/UI/MainGUIViewPresenter.cs). Keep in mind that we cannot provide the code for the NGUI library, so you’ll need add your own copy to the Assets/Libs folder in the project if you want to compile and run it. We will refer to this example through the rest of the post, so feel free to download the code an check it while you read.
## General overview
Here’s a general diagram that shows how the different parts of this pattern interact in a high level view. A detailed explanation for each component follows.
![General Overview Diagram](http://engineering.socialpoint.es/images/mvc_ngui/overview.png)
### Model
The traditional MVC `Model` we all know and love:
- Holds no view data nor view state data.
- Is accessed by the `Controller` and other `Models` only
- Will trigger events to notify external system of changes.
The Model is implemented with [Plain Old C# Objects (POCOs)](http://en.wikipedia.org/wiki/Plain_Old_CLR_Object)
[Here's a link to the `PlayerModel` class representing Player data](https://github.com/socialpoint/engineering-blog-examples/blob/master/MVC/NGUI/Assets/Scripts/Models/PlayerModel.cs). The `PlayerModel` class stores several data for the player, including HitPoints, XP and its Level, accessible using two properties. We can add XP points, an action that raises an `XPGained`event from the model. When we trigger enough experience to advance a level, the `Level` property is updated and raises a`LevelUp` event.
### View
Conceptually the `View` is just something that will be rendered in the screen. The responsibilities of a view are:
- Handle references to elements needed for drawing (Textures, FXs, etc)
- Perform Animations
- Layouts
- Receive User Input
In this particular case the View is implemented using NGUI, so it is just a prefab inside the Unity project. But that’s an implementation detail that we need to decouple, more on this later. Getting more academic this is a case of a [Passive View](http://martinfowler.com/eaaDev/PassiveScreen.html). This means that the view knows nothing about other parts of the project, either data or logic. Thus, some other code must explicitly tell the view what to display, what animation to play, etc.
### Controller
The `Controller` is the link between the Model and the View. It holds the state of the View and updates it depending on that state and on external events:
- Holds the application state needed for that view
- Controls view flow
- Shows/hides/activates/deactivates/updates the view or parts of the view depending on the state. For example the controller can temporarily disable the special attack button because the player is in the cooldown time, after that time the controller re-enables it.
- Load/Instantiate needed assets, for example to show particles, change sprites dynamically, etc
- Handles events either triggered by the player in the View (e.g. the player touched a button) or triggered by the Model (e.g. the player has gained XP and that triggered a Level Up event so the controller updates the level Number in the view)
These are the three basic elements that defines a MVC pattern. However, in this case we felt we need to add another indirection layer to further decouple the specific NGUI View implementation from our code. We call that part:
### ViewPresenter
A `ViewPresenter` sits between the View and the Controller and it is an interface that will expose common operations that are generic to a View, no matter how is implemented internally.
For example, a button to be used in a game can have the following functionality:
- Set the text for the button’s label
- Change the background image
- Enable / disable user input
- Notify when an user clicks the button
These are implementation-independent operations that you can find in every button in any UI toolkit. It is implemented as a MonoBehaviour and it will be attached to an NGUI View prefab, so we can use the `GameObject.GetComponent()` method to retrieve it in order to access to the functionality it provides from the Controller. As the `ViewPresenter` is the bridge between or game code and UI code it cannot be completely independant of the underlying implementation which is used to render things on the screen. In our case it will need to keep references to NGUI widgets (`UIButton`, `UILabel`, etc) in order to interact with them. The `ViewPresenter` is basically an [Adapter pattern](http://en.wikipedia.org/wiki/Adapter_pattern): we create our custom interface in order to access code external to our application. Those references must be set explicitly using either code or the inspector
![View Presenter in the Unity3D Inspector](http://engineering.socialpoint.es/images/mvc_ngui/view_presenter.png)
Fortunately it can be partially automated: [please check out the `ViewPresenter.AutoPopulateDeclaredWidgets()` method in the sample code for further info](https://github.com/socialpoint/engineering-blog-examples/blob/master/MVC/NGUI/Assets/Scripts/UI/ViewPresenter.cs#L91). Although the `ViewPresenter` is coupled to an specific UI system we create an interface consumed by our Controller which is a big gain: if we need to change the GUI toolkit used to render things in the screen we only need to modify the implementation keeping the public API untouched so we don’t need to touch any controller logic.
It is called `ViewPresenter` because it is similar in concept to the Presenter in a [Model-View-Presenter pattern](http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter), but with one important difference: the `Presenter` is allowed to access the `Model` while the `ViewPresenter` can't.
[Click here to see the example code of a PlayerController that will handle the view for the XP system](https://github.com/socialpoint/engineering-blog-examples/blob/master/MVC/NGUI/Assets/Scripts/Controllers/PlayerController.cs)
The `ViewPresenter` can hold some state if it is related to the way things are presented to the player. For example, the view could store the value of different colors to tint the Health Point indicator and change it depending on the health level. Those values can even be exposed as public properties to allow changing them in realtime using the inspector. However it must not hold any application logic state, leave that to the `Controller`, the `ViewPresenter` should not even know when a health level is considered “low”.
For example, if we extend our `PlayerController` to handle the Hit Points we can add a method that will change the color of the label when it is in low health:
public class PlayerController
{   // ...
    void UpdateHitPointsUI()
    {
        if (Player.HasLowHitPoints)
        {
            HitPointsViewLabel.ShowLowHealthColor();
        }
        else
        {
            HitPointsViewLabel.ShowNormalHealthColor();
        }
    }
}
This approach can be a bit overkill unless you want to create a very specific or complex custom widget and reuse it in your project. In the sample code we go for the easy route: the controller just changes a `UnityEngine.Color` property in the `ViewPresenter`.
## Handling UI events
NGUI provides a nice event system that allows triggering a method in any `MonoBehaviour` we define as the handler of the event. This decouples the MonoBehaviour that generates the event from the one that will handle it. However, with great power comes a great chance of messing up all the architecture; as you can use ANY `MonoBehaviour` as a handler, it's really easy -and handy- to link in the inspector whatever `MonoBehaviour` you have in your scene to get things done, creating a dependency that can be difficult to track when you have a complex View created using several controls. Our dependency graph can become complex really fast.
To prevent Chaos from spreading among our codebase we follow a really easy rule: All View UI events will be handled by the ViewPresenter attached to that View. The ViewPresenter will capture the NGUI events and then raise a vanilla .NET event in response. The rest of the code subscribes to that .NET event. We do it so because that way we decouple the specific implementation of UI events from NGUI, and because this way we have the events wired by code, not in the inspector. This is -in my opinion- a safer approach: is more explicit, you can easily search for the code that handles that event using your IDE, is type-safe (if you delete the reference to the `MonoBehavior` that handles the event you’ll only notice when your control stop working in play mode) and allows to set the arguments we want to send with the event. Of course we need to wire the NGUI event with the ViewPresenter but we can automate it: [check the `ButtonViewPresenter.WireUIEvents()` method in the example code](https://github.com/socialpoint/engineering-blog-examples/blob/master/MVC/NGUI/Assets/Scripts/UI/ButtonViewPresenter.cs#L119)
## Creating complex views.
Now that we have some building blocks, it is easy to create more complex views by composition: add a new view formed by several UI prefabs, and create a new ViewPresenter for that composite view exposing the child ViewPresenters this view uses so you can access them from a controller:
![Complex view in Unity3D inspector](http://engineering.socialpoint.es/images/mvc_ngui/complex_view2.png)
![Complex view in Unity3D inspector](http://engineering.socialpoint.es/images/mvc_ngui/complex_view1.png)
Check out the code for this view [by clicking here](https://github.com/socialpoint/engineering-blog-examples/blob/master/MVC/NGUI/Assets/Scripts/UI/MainGUIViewPresenter.cs).
### Finishing words
Please, tell use what do you think of this approach, or tell us about your approach when creating UIs for your game. Feedback is always welcome!
By the way, if you don’t have NGUI it would be a great exercise to change the example project to use the immediate GUI API from Unity3D: you just need to replace the NGUI Widget references and implement your OnGUI() method in each `ViewPresenter`. Don't hesitate to send us a pull request if you decide to do it ;)

