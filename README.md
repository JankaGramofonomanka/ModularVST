# ModularVST

I About:

This is a VST with a collection of CV (control voltage) modules to build a synth out of them, 
those modules are supposed to reflect the way modules in a modular synthesizers are build.

II Setting up the project:

if you are on Linux you might want to read this:
https://steinbergmedia.github.io/vst3_doc/vstinterfaces/linuxSetup.html

Create a diretory {DIRECTORY}
navigate to {DIRECTORY} in the terminal, and copy the vst3sdk repository by entering the following in the terminal:
> git clone --recursive https://github.com/steinbergmedia/vst3sdk.git

in {DIRECTORY} create a folder called 'my_plugins' 
enter {DIRECTORY}/my_plugins and create a file 'CMakeLists.txt' and add to it the following line:
> add_subdirectory(ModularVST)

Now you can build the plugin, to do this, go back to {DIRECTORY} and follow these insstructions:
https://github.com/steinbergmedia/vst3sdk/blob/master/README.md (section 'How to build VST3')
(of course do not clone the repository again)

The plugin will be created in the folder {DIRECTORY}/build/VST3/Debug
