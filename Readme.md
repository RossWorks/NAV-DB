# Navigation Database generation tool
## What is it
This tool is part of the NavSys suite.<br>
Its goal is to generate a Navigation Database for the navigation system to be queried.<br>
From the database, the navigation system shall be able to extract all the info needed to accurately plan and navigate a path around the Earth.
## what it does
the basic functions of this tool are
 - Acquire the info hold by Arinc 424 files
 - Browse those data to let the user inspect the data before final processing
 - Transfer those information into a single, specialized file for the NavSys to query
## How to use it
### Startup
Launch this tool from the terminal, passing as argument a `.ini` file containg settings (see examples).<br>
The Tool will begin reading all files contained in a specified folder.
At the end of the loading phase, the tool allows the user to enter a search key.
### Search
The Search key must be Upper case (all strings in Db are Uppercase). The Tool will search the Db for any point which ICAO code mach exactly the search key.<br>
When the search is completed, the user is presented with a brief list of the search result. To get a more detailed look of one of the search results, type "#" followed by the number  of the search result to inspect
### Closing
Just type `QUITNOW` at any time to close the generation tool.