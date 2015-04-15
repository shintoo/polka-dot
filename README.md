#polka-dot
A simple dotfile and configuration manager.

##Installation
Run `sh compile` to install polka-dot

##Usage
polka-dot comes with four commands: `save`, `apply`, `rm`, and `list`. The former two work with a config file that must be written, containing a path to the file to be packaged.
Example config file:
```
~/.vimrc
~/.Xdefaults
~/.i3/config
```

###save
`polka-dot save package-name`  
`save` will package the files listed in the config file into a polka-dot *package*.

###apply
`polka-dot apply package-name`  
`apply` will apply the polka-dot package to the files listed in the config file.

###rm
`polka-dot rm package-name`  
`rm` will remove the polka-dot package from polka-dot.

###list
`polka-dot list`
`list` will list all of the currently saved packages.

##Notes
polka-dot will not save your current configuration when `apply` is used, so make sure to `save` before you `apply` if you wish to save your current configuration before changing!
