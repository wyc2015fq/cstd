# [React] Implement a React Context Provider - weixin_33985507的博客 - CSDN博客
2017年12月18日 15:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
If you have state that needs to exist throughout your application, then you may find yourself passing props all over the application and even "drilling" the prop through components that don't really care about the prop at all. In this lesson, we'll see a sample of a small app that has the "prop drilling problem" and learn how to implement the "Provider pattern" to access context state anywhere in the component tree.
To implement a context provider for render props:
```
class ToggleProvider extends React.Component {
  static contextName = '__toggle__'
  static Renderer = class extends React.Component {
    static childContextTypes = {
      [ToggleProvider.contextName]:
        PropTypes.object.isRequired,
    }
    getChildContext() {
      return {
        [ToggleProvider.contextName]: this.props
          .toggle,
      }
    }
    render() {
      return this.props.children
    }
  }
  render() {
    const {
      children,
      ...remainingProps
    } = this.props
    return (
      <Toggle
        {...remainingProps}
        render={toggle => (
          <ToggleProvider.Renderer
            toggle={toggle}
            children={children}
          />
        )}
      />
    )
  }
}
function ConnectedToggle(props, context) {
  return props.render(
    context[ToggleProvider.contextName],
  )
}
ConnectedToggle.contextTypes = {
  [ToggleProvider.contextName]:
    PropTypes.object.isRequired,
}
```
Modify the code:
