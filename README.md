#polka-dot
A simple dotfile and configuration manager.

##Installation
Run `sh compile`, then move the resulting executable to your PATH.

##Usage
polka-dot comes with some commands:
- `save`  
- `apply`  
- `del`  
- `status`  
- `list`  
- `queue`
- `add`
- `reset`  
  
###Managing Packages  
####save
`polka-dot save package-name`  
`save` will package the files listed in the queue into a *package*.

####apply
`polka-dot apply package-name`  
`apply` will apply the package to the files in the package.

####del
`polka-dot del package-name`  
`del` will remove the package from polka-dot.

####status
`polka-dot status package-name`  
`status` will list all the files saved in the package.  

####list
`polka-dot list`  
`list` will list all of the currently saved packages.

###Managing Queue  
####queue
`polka-dot queue`  
`queue` will list all of the files currently queued for packaging.  

####add
`polka-dot add path-to-file`  
`add` will add the path to the queue of files to be packaged.  

####reset
`polka-dot reset`  
`reset` will clear the queue.  

##Notes  
- polka-dot will not save your current configuration when `apply` is used, so make sure to `save` before you `apply` if you wish to save your current configuration before changing!  
- Many programs require some sort of reload or restart to apply their config. To see the effects of `apply`, make sure to do any required refreshes.
 
