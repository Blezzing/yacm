# YACM
Yet Another Configuration Manager

As i was frustrated manually managing configuration files located multiple places on my systems, i wanted to make a little script to make this easier. It evolved into to this slightly overblown solution.

As there exist other tools to do similar task i named this one YACM, as my naming skills are pretty bad.

# Setup
Currently the only way to get `yacm` is by compiling it yourself. I have tried to make this simple, and please leave issues if you run into trouble.

## Compilation
Compilation requires a relatively new version of g++, as it uses the STL filesystem features.

```bash
cd wherever_you_cloned_it_to
make
```

## PATH
Afterwards symlink/move/copy it to a folder included in your `PATH` variable, or add the folder to your `PATH`.

eg. linking:
```bash
sudo ln -s ~/git/yacm/yacm /usr/bin/yacm
```

# Usage
To use `yacm`, you need to have a folder you intend as the root for all your configuration file backups, such as `~/git/dotfiles`. In this folder you have to have a file named locations.toml, which is going to list your files, and relations between the backup and real ones.

## Model
The model of `yacm` is consisting of two types, `element`s and `group`s.

`element`s are representations of a single configuration file, they have a name, and two locations, representing where they should be on the system, and where they should be in the backup folder.

`group`s are representations of multiple elements you always want to consider together. It could be a group of files describing a colorscheme in different languages, or all the bash scripts you kept around for far too long.

Whenever you use `yacm`, you address files by their group.

## locations.toml
This file is manually created and modified. It is in Tom's Obvious Minimal Language, and need to have the following structure:

```toml
[group-name]
element-name = ['path-on-system', 'path-in-backup']
```

With the names representing
|Element|Description|
|:---|:---|
|`group-name`|The name of a group.|
|`element-name`|The name of an element in the previously mentioned group.|
|`'path-on-system'`|The path to where the configuration file should be in a working system.|
|`'path-in-backup'`|The path to where the configuration backup should be stored.|

### Example locations.toml

```toml
[herbstluftwm]
xinitrc = ['~/.xinitrc', 'herbstluftwm/.xinitrc']
autostart = ['~/.config/herbstluftwm/autostart', 'herbstluftwm/autostart']

[bash]
#etc
```

The example is chosen to show how this can help with storing different versions of `.xinitrc`, based on the window manager in use, to store more than a single setup in the same backup directory. It also shows the support for `~` which will be unwrapped to your `HOME` or `USERPROFILE` variable.

### Notice
If you chose to use anything but `ASCII` in your location.toml, you ~~might~~ **will** have a bumpy ride. Same goes for spaces in names.

## Commands
The syntax is pretty simple:

```bash
yacm [option] [group(s)]
```

Where `option` must be specified, as it descripes the wanted action, and any number of groups can be specified to perform the action on.

`option` can be:

|Option|Description|
|:---|:---|
|list|Show the result of parsing the toml file. Can be used with groups to perform dummy searches.|
|save|Copies files from the system to the backup directory.|
|load|Copies files from the backup directory to the system.|

### Example commands

#### Example 1
```bash
yacm save herbstluftwm
```

Will make a copy the two elements, `xinitrc` and `autostart`, from their system locations to their backup locations.

#### Example 2
```bash
yacm load
```

Will make a copy of **every single element** described the toml file, from their backup locations to their system locations. 

# Features and wishlist
"He knows da wey!" -A clever red guy

## Current features:
* A way to handle multiple configuration files at once with groups
* A way to handle 

## Future/Wanted features:
* A way to handle specific configuration files at a time
* A way to detect and resolve conflicts if multiple elements share a location in system or backup
* A way to check if some files have been changed in one of the places
* A way to store information about where your backup location is, to avoid excessive use of `cd`

## Wont-get features
* Any kind of remote hosting. Use GitHub or something.

# Thank you
Thank you to Skystrife for making the toml library [cpptoml](https://github.com/skystrife/cpptoml). It made this project much easier to start.