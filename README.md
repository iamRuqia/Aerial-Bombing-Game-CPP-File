# Aerial-Bombing-Game-CPP-File
 (بمباران هوایی)
=====================================================================================================================================

A classic terminal-based aerial bombardment game written in C++ using Windows Console API. Control an airplane, drop bombs on buildings, and navigate through a cityscape while avoiding collisions.

🎮 Features
=====================================================================================================================================

1_Retro Console Graphics – Uses Windows Console Buffer for smooth rendering
2_Dynamic Gameplay – Adjustable flight speed, altitude management, and bombing mechanics
3_Collision Detection – Avoid buildings with your airplane and wingtips
4_Scoring System – Earn points for successful hits, lose points for misses
5_Persian Interface – Menu and instructions in Persian (Farsi)
6_Real-time Feedback – Visual and audio feedback for hits/misses

🛠️ Controls
=====================================================================================================================================

1_Spacebar – Drop bomb
2_Up/Down Arrow – Increase/decrease speed (1-3 levels)
3_Q – Quit game

📊 Game Mechanics
=====================================================================================================================================

1_Altitude decreases over time – maintain height by managing speed
2_Buildings have random heights (2-6 floors)
3_Score = (building floors × 10) for hits, -5 for misses
4_Game ends if you crash into buildings or descend too low

🖥️ Technical Highlights
=====================================================================================================================================

1_Object-oriented design with GameObject inheritance
2_Double-buffered console output for flicker-free animation
3_Custom rendering system using CHAR_INFO buffer
4_Real-time keyboard input handling

=====================================================================================================================================

Perfect for demonstrating console game development, C++ OOP principles, and real-time interactive systems in a lightweight terminal environment.
Why it's cool: This isn't just another Snake game – it's a full aerial combat simulator with physics, collision systems, and retro aesthetics, all in the Windows console! Great example of what you can build with native Windows APIs and clever programming.
