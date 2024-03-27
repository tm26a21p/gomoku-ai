# Gomoku: Smart Bots for a Simple Game

## Technical Overview

### General Information
- **Binary Name:** pbrain-gomoku-ai
- **Languages:** C, C++, Python 3 (standard distribution only)
- **Compilation:** Via Makefile, including `re`, `clean`, and `fclean`
- **Repository Structure:**
  - Source files included, excluding unnecessary files like binaries, temp files, and obj files.

### Project Objective
Implementation of a Gomoku Narabe game bot (Five in a Row) focusing on its artificial intelligence performance. The bot complies with the communication protocol (mandatory part only). Challenge other bots to evaluate your bot's strength.

### Algorithm
I used the a version of the Monte-Carlo Tree Search (MCTS) algorithm to develop the bot. The algorithm is a heuristic search algorithm that uses random sampling to find the best move. The algorithm is divided into four steps:
1. **Selection:** Traverse the tree from the root node to the leaf node.
2. **Expansion:** Create one or more child nodes from the leaf node.
3. **Simulation:** Simulate a random playout from the child node.
4. **Backpropagation:** Update the node statistics based on the simulation result.

### Technical Constraints
- Programs must compile on both Windows (for the final tournament) and Linux (for automated tests) using the provided Makefile.
- Only standard libraries are allowed; no TensorFlow or scikit-learn.

### Game Rules
- A 2-player game played on a 20x20 game board (goban).
- Players take turns placing stones.
- The game ends when one player achieves 5 stones in a row (vertically, horizontally, or diagonally).

### Grand Tournament
Evaluation of the bot is based on:
1. **Play to Win:** Understand winning situations and make the right moves.
2. **Outsmart Local Bots:** Compete against low to medium-level bots to maximize victories.
3. **Tournament Phase (Maybe):** Top 16 bots compete, and the best score wins the tournament.

### Tournament Rules
- 5 seconds maximum per move.
- 70 MB of memory per bot.
- A forbidden move leads to automatic defeat.
- Using a forbidden library leads to elimination.

The tournmanent took place collecting each group's AI bots and running them against each other.

## Conclusion
I learned a lot about the MCTS algorithm and its implementation. it was a real challenge to develop a bot that could compete with other bots.  
     
Through this project, I learned proficiency in C/C++ programming, algorithm optimization (and I love it!), and problem-solving skills.