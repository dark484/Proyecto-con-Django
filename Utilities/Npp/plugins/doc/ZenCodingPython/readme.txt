Zen Coding for Notepad++ - Python Version



This plugin is powered by the Python Script plugin for Notepad++ - install that with Plugin Manager, 
or download from http://npppythonscript.sourceforge.net or http://www.brotherstone.co.uk/npp/ps

You need the Visual C++ Runtime 2008 for the Python Plugin 
(this is a Python requirement - hopefully this will change in the future)
If you've not got it, you'll get a LoadLibrary failure (chances are you have, lots and lots of software 
comes with it).  Download it free from Microsoft:

http://www.microsoft.com/downloads/details.aspx?FamilyID=9b2da534-3e03-4391-8a4d-074b9f2bc1bf&displaylang=en


Zen Coding is Copyright and developed by Sergey Chikuyonok



This plugin is Copyright (C) 2010 Dave Brotherstone

License: GPL2

Source available on github.com/davegb3



Installation
============

Just copy the ZenCoding-Python.dll to your Plugins directory (normally c:\Program Files\Notepad++\plugins), and the ZenCodingPython
directory to your plugin config directory - normally %APPDATA%\Notepad++\plugins\config\ZenCodingPython, but can also be
under plugins\config from your Notepad++ directory (if you have a doLocalConf.xml file next to notepad++.exe)

	
Usage
=====
Set the shortcuts under the shortcut mapper (Settings menu)
If you set the shortcut for Expand Abbreviation to just "Tab", you need to remove the Tab shortcut from the Scintilla commands (for SCI_TAB).
Notepad++ won't let you do this, so you have to change the shortcut to something obscure (Ctrl-Alt-Shift-9 or something)

If you set Tab as the shortcut, it will intellegently expand an abbreviation if one is there, or if not, send the Tab command directly to Scintilla
(so won't interfere with normal Tab operation, eg. indenting and so on)

Profiles are automatically selected when the Auto Select Profile is on.  This switches between xhtml (default), xml when the language is set to XML,
and Plain, when the language is Normal Text.  

Edit Shortcuts - this allows you to easily edit your own shortcuts - this file is not replaced by Plugin Manager (when Zen Coding for Python is available 
in Plugin Manager), so you can add your own abbreviations to this, and they won't be overwritten.



Contact 
=======

Comments, bug reports, requests etc as always, welcome - davegb@pobox.com





Version history
Version: 
		0.1 
			- Initial relese
			
        0.1.1
		    - ZenCodingPython files updated with correct structure
			
		0.2
			- Profiles available, with auto select
			- Edit settings option, with automatic refresh (when saved from Notepad++)
			- my_zen_settings.py now auto-generates, to allow non-installation by Plugin Manager.
			- Tab key support
			- XSD mode and filter added
			