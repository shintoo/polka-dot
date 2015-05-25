#polka-dot
A simple dotfile and configuration manager.  
  
polka-dot allows you to save multiple config files into a single package and save it, so you are free to experiment with your configs without worry. Also, you can save multiple packages of config files, and swap out which are active with one command. For example, you can save your configs for your window manager, terminal, homepage, and GTK into a package, download or create new versions, and save that as another package. Then, you can easily switch between the two packages, changing the entire look of your desktop with one command.

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
A *package* is a selection of files (specified in the *queue*) saved together in an archive.  

####save
`polka-dot save package-name`  
`save` will package the files listed in the queue into a package.

####apply
`polka-dot apply package-name`  
`apply` will apply the package to the files listed in the queue.

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
The queue is the list of files to be saved into a package.  

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
- Currently, the files in the package must match the files in the queue. To do this, use `status` and `add` each file. Currently working on a fix.
 
