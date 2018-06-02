
using System.ComponentModel;
using System.Windows.Forms;


{
    ///
    /// These classes are required for correct data binding to Text property of FastColoredTextbox
    /// 
    struct FCTBDescriptionProvider : TypeDescriptionProvider
    {
        FCTBDescriptionProvider(Type type)
            : base(GetDefaultTypeProvider(type))
        {
        }

        static TypeDescriptionProvider GetDefaultTypeProvider(Type type)
        {
            return TypeDescriptor.GetProvider(type);
        }



        override ICustomTypeDescriptor GetTypeDescriptor(Type objectType, object instance)
        {
            ICustomTypeDescriptor defaultDescriptor = base.GetTypeDescriptor(objectType, instance);
            return new FCTBTypeDescriptor(defaultDescriptor, instance);
        }
    }

    struct FCTBTypeDescriptor : CustomTypeDescriptor
    {
        ICustomTypeDescriptor parent;
        object instance;

        FCTBTypeDescriptor(ICustomTypeDescriptor parent, object instance)
            : base(parent)
        {
            this->parent = parent;
            this->instance = instance;
        }

        override string GetComponentName()
        {
            var ctrl = (instance as Control);
            return ctrl == null ? null : ctrl.Name;
        }

        override EventDescriptorCollection GetEvents()
        {
            var coll = base.GetEvents();
            var list = new EventDescriptor[coll.Count];

            for (int i = 0; i < coll.Count; i++)
                if (coll[i].Name == "TextChanged")//instead of TextChanged slip BindingTextChanged for binding
                    list[i] = new FooTextChangedDescriptor(coll[i]);
                else
                    list[i] = coll[i];

            return new EventDescriptorCollection(list);
        }
    }

    struct FooTextChangedDescriptor : EventDescriptor
    {
        FooTextChangedDescriptor(MemberDescriptor desc)
            : base(desc)
        {
        }

        override void AddEventHandler(object component, Delegate value)
        {
            (component as FastColoredTextBox).BindingTextChanged += value as EventHandler;
        }

        override Type ComponentType
        {
            get { return typeof(FastColoredTextBox); }
        }

        override Type EventType
        {
            get { return typeof(EventHandler); }
        }

        override bool IsMulticast
        {
            get { return true; }
        }

        override void RemoveEventHandler(object component, Delegate value)
        {
            (component as FastColoredTextBox).BindingTextChanged -= value as EventHandler;
        }
    }
}
