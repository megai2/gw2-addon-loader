---
layout: update
author: Archomeda
title: Beta testing
date: 2018-03-07 19:00:00 +0100
published: false
---
Guild Wars 2 Add-on Loader is a new add-on for Guild Wars 2 to load your precious add-ons, but without much of a hassle.
The development is not finished yet, but I'm starting the beta testing as of today.
This means that other add-on developers have time to make their add-ons compatible, provide API feedback, report issues, etc.
Of course, you as a Guild Wars 2 player can use it already too, but keep in mind that the add-on loader on its own doesn't do much.
Luckily there are a few small add-ons available already that are made by me.

There are currently two add-ons included:
 - **Fix Mouse Movement**, which fixes a small annoyance with the (still moving) mouse cursor while you're rotating your camera.
 - **TacO Launcher**, which does exactly what it says: launches TacO automatically.
   The requirement is that the entire TacO folder should be placed in the *&lt;Guild Wars 2 folder&gt;/bin64/addons* or *&lt;Guild Wars 2 folder&gt;/bin/addons* folder.

Other well-known add-ons are not yet available, but keep an eye on them.
It's up to their respective developers to make them compatible.
Any additional add-on that will be made available, has to be placed in the *&lt;Guild Wars 2 folder&gt;/bin64/addons* or *&lt;Guild Wars 2 folder&gt;/bin/addons* folder.

Proper documentation is scarce right now, but it will become more detailed as the add-on loader approaches its proper release.
Add-on developers can look at the *README.md* file that's included in the root of the repository for more information.

**Keep in mind** that right now, ArenaNet has **not yet** authorized the usage of this add-on.
By using this add-on, you assume all responsibility.
I've reached out to Chris Cleary, but I've yet to receive an answer.
That said, this add-on doesn't do much on its own.
It doesn't interact with the gameplay of Guild Wars 2 at all.
It only draws an overlay on top of Guild Wars 2, and it intercepts keyboard and mouse calls in order to allow interaction with the in-game add-on GUI.
However, it also forwards a bunch of calls to allow other add-ons to work.
Think of it as chainloading, but better.
Therefore, I'm not responsible for any other add-on that is developed without my input.
Nor does ArenaNet authorize the usage of them, as they can still be against the Terms of Service.
Any add-on that you use, can have full control over your game.
So be mindful with what you use.

With that out of the way, you can install this add-on by [downloading it from the repository release page][releases], and then by placing the *d3d9.dll* file in the *&lt;Guild Wars 2 folder&gt;/bin64* or *&lt;Guild Wars 2 folder&gt;/bin* folder.
It's that simple.
If it somehow causes crashes (which isn't unlikely), just rename the file to something else, or delete it in its entirety.
Don't forget to [report it in the issues section of the GitHub repository][issues]!

If you feel fancy, you can also check out [the source code on GitHub][repository]!

[repository]: {{ site.github.repository_url }}
[releases]: {{ site.github.releases_url }}
[issues]: {{ site.github.issues_url }}
