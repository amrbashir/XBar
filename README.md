<p align="center">
    <img src="XBar.png" >
    <br />
    <img src="https://img.shields.io/github/license/amrbashir/xbar" />
    <img src="https://img.shields.io/github/v/release/amrbashir/xbar" />
    <img src="https://img.shields.io/github/workflow/status/amrbashir/XBar/CI/master" />
</p>

# XBar

A tool to change windows 10 taskbar appearance with per-app rules.

## Motivation
Although TranslucentTB is a great project, it doesn't provide per-app taskbar color so I decided to make XBar to provide per-app rules which you can configure in the config file located at `%AppData%/XBar`.

Having rules makes it possible to have seamless integration between the taskbar and the titlebar of an app if you want.

## Screenshots
Given this config :
```jsonc
{
    "general": {
        // whether to run the app at windows startup
        "runAtStartup": true,
        // whether to show or hide the tray icon
        "showTrayIcon": true
    },
    "maximized": {
        // can be one of "opaque", "transparent", "fluent", "normal", "blur"
        "accentState": "opaque",
        // can be 3-digit hex color like "#fff" or 6-digit like "#ffffff"
        // or 8-digit that contiains alpha value like "#BFffffff"
        "color": "#1f1f1f",
        "rules": {
            // the rules are basically key value pair
            // where the key is the exeName
            // and the value is the color followed by a slash followed by accent state, ie "color/accentState"
            "discord.exe": "#202225/opaque",
            "vivaldi.exe": "#14151B/opaque",
            "firefox.exe": "#202340/opaque"
        }
    },
    "regular": {
        "accentState": "transparent",
        "color": "#00000000",
        "rules": {}
    }
}

```
**Desktop**
![desktop1](screenshots/desktop1.png)
![desktop2](screenshots/desktop2.png)
**Apps**
![app1](screenshots/app1.png)
![app2](screenshots/app2.png)
![app3](screenshots/app3.png)

## TODO :

- [ ] Detect Areo Peek
- [ ] Detect Start Menu
- [ ] Detect Task View
- [ ] Contribute back to [TranslucentTB](https://github.com/TranslucentTB/TranslucentTB)

## Contributing or Building from source.

See the [CONTRIBUTING.md](CONTRIBUTING.md) .

## Thanks
- [TranslucentTB](https://github.com/TranslucentTB/TranslucentTB) - Great project, XBar wouldn't be possible without it.

## License
Licensed under the GPLv3 License. Please see the [LICENSE.md](LICENSE.md) file for more.
