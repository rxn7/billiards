# Billiards
This is a game of billiards (pool) written in C++ using SFML for rendering and event handling and GLM for quaternion/matrix maths.

# Build dependencies
- SFML (2.5)
- GLM
- ImGUI
- ImGUI-SFML
- glslang (to validate the shaders during build)

# Build
Command: ```./scripts/build.sh```  
Args:
- Build for linux (default): ```-p linux```  
- Build for windows: ```-p windows```   
- Build release ```-r```  

# Run
Command: ```./scripts/build.sh```  
Args:
- Run linux executable (default): ```-p linux```  
- Run windows executable (wine): ```-p windows```   
- Run release version ```-r```  

# Example build and run:
Build and run windows release:   
```./scripts/build.sh -p windows -r```   
```./scripts/run.sh -p windows -r```   
   
Build and run linux debug:    
```./scripts/build.sh -p linux``` or  ```./scripts/build.sh```  
```./scripts/run.sh -p linux``` or ```./scripts/run.sh```   

# Devlogs
I document the progress on my youtube channel: https://youtube.com/playlist?list=PLVsn5gXfgBQcwPg6fdQcR0NCpo5vr9-6O 
