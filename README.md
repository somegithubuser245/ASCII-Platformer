# Simple ASCII Platformer
## Simple game, Player needs to reach to symbol 'O' to win
### How to start?
  - Compile the game using txt/COMPILE.txt
  - You will land on the start screen with preinstalled maps
  - Choose one and start Playing!

### Controls
  - Use A and D and press Enter to move Right/Left
  - Use F and press Enter to climb the ladder
  - E to exit the game
  - To create your own Maps, upload them in /maps/ Folder

### The Rules for your own map are as follows:
 - Y and X at the start of the map.txt file
 - Y - height, X - width
 - Ladders should not end in empty space.
 - There should be enogh Space between Platforms for Player (MAX_SPACE, 3 by default)
 - There should be Start position (S) and end position (O), 2 cells above the ground

Your map will be checked by the programm and warn you if something's wrong with uploaded map
### Example of the map.txt file:

```
15 30
 S                       O  
                            
---------   -------- ------- 
    H                 H     
    H                 H     
    H                 H     
    H            ------------
 --------         H         
       H          H         
       H          H         
       H          H          
      ----------------      
```
