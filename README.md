# 🚀 OmniCalc v4.2: Engineering-Grade Hybrid Engine

OmniCalc is a high-performance, C-based hybrid calculator and symbolic derivative engine designed for engineers and technical professionals. It bridges the gap between standard arithmetic operations and academic-level calculus.

## 🛠️ Key Features
- **Symbolic Derivative Engine:** Solves complex equations like `sin^2(3x)*cos^6(4x)` using advanced Chain Rule logic.
- **Hybrid Functionality:** Seamlessly switch between daily arithmetic (n-count addition/subtraction) and symbolic mathematical analysis.
- **Advanced Parser:** Features an intelligent Recursive-Descent parser that understands implicit multiplication (e.g., `3x` or `3sin(x)`).
- **Precision:** v4.2 update provides full stabilization for floating-point inputs (e.g., `0.5`, `.25`).

## 📐 Usage Guide
| Operation | Syntax | Example |
| :--- | :--- | :--- |
| **Power / Square** | `^` | `x^2` |
| **Trig Powers** | `sin^n(x)` | `sin^3(2x)` |
| **Multiplication**| `*` or Implicit | `3*x` or `3x` |
| **Division** | `/` | `sin(x)/x` |

## 💻 Tech Stack & Algorithms
- **Language:** C
- **Data Structures:** Abstract Syntax Trees (AST)
- **Methodology:** Recursive Descent Parsing & Symbolic Differentiation Algorithms

## 🚀 Installation & Running
1. Clone the repository or download `omniCalc.c`.
2. Compile with any standard C compiler (GCC recommended):
   ```bash
   gcc omniCalc.c -o omniCalc -lm
