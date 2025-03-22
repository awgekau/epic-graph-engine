# 🧠 Epic Graph Engine

A dynamic graph implementation in **C**, designed for handling node relationships and weighted edges in large-scale, open-ended systems like story-driven games, simulation engines, or learning tools. Built with dynamic memory allocation and optimized for memory safety using Valgrind.

---

## 📸 Visualization

### Adjacency List Representation

<img src="assets/adjlists.png" alt="Adjacency List Example" />

### Graph Traversal Example

<img src="assets/animation.gif" alt="Graph Traversal Example" />

---

## 🚀 Features

- ✅ Directed & weighted graph representation  
- ✅ Supports dynamic node and edge insertion/deletion  
- ✅ Path traversal with edge weight accumulation  
- ✅ Memory leak-safe (Valgrind tested)  
- ✅ Fast adjacency list design  
- ✅ Clean command-based input interface  

---

## 🧩 How It Works

This graph engine processes commands via `stdin` or redirected file input. It dynamically manages memory for nodes and edges using adjacency lists. Each node can connect to multiple destinations with associated weights.

---

## 💻 Commands Supported

| Command | Description |
|--------|-------------|
| `i <id>` | Insert node |
| `d <id>` | Delete node and all connected edges |
| `l <src> <dst> <weight>` | Link: add or update edge |
| `r <src> <dst>` | Remove edge |
| `n <id>` | Check if node exists |
| `e <src> <dst>` | Check if edge exists |
| `p <id_1> <id_2> ... <id_k>` | Traverse path and sum edge weights |
| `t` | Terminate and free memory |

---

## 📦 Building & Running

### Compile

```bash
gcc -Wall -Werror -std=c99 src/ex5q1.c -o graph
