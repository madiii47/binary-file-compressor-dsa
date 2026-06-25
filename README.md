# Binary File Compressor (Custom DSA Implementation)

A low-level binary file compression and decompression utility engineered in C++. The application leverages a byte-level Run-Length Encoding (RLE) algorithm. To maximize execution control and demonstrate foundational knowledge of data structure architecture, all storage elements—including heaps, trees, and queues—were implemented entirely from scratch without relying on the C++ Standard Template Library (STL).

---

## Technical Architecture & Custom Data Structures

Instead of using standard library definitions, this engine operates via custom memory-managed structures:

* **Custom Min-Heap:** Handles run-length storage array mechanics. It utilizes sequential index array positioning to dynamically perform up-heap and down-heap balancing routines when tracking the minimal byte sequences.
* **Custom Binary Search Tree (BST):** Tracks unique character structures during streaming passes. Features automated recursive pointer traversal loops and safe post-order deep-memory deletion routines to manage dynamic node cleanup.
* **Custom Linked-List Queue:** Manages sequential raw byte queues during file translation. Operates via persistent Head/Tail pointers to offer stable $O(1)$ object processing boundaries.
* **Binary Stream Serialization:** Uses native sequential input/output streams (`ios::binary`) to read raw file bytes, writing tracking headers (`"RLE1"`) to prevent corrupted data translation loops.

---

## Technical Specifications

* **Language:** C++
* **Encoding Threshold:** Dynamic 8-bit parsing caps (`count < 255`) to safely match maximum character allocation bounds.
* **Memory Protocol:** Strict manual pointer deallocation cycles handled inside clear functional scopes to eliminate data leaks.

---

## How to Setup and Run

1. **Clone the Project:** Download the source repository code file to your local workspace.
2. **Compile the Implementation:** Run a standard C++ compiler layout inside your command console or development environment (like Dev-C++ or VS Code):
   ```bash
   g++ main.cpp -o FileCompressor
3. **Run the Utility:** Run the compiled binary output executable:
   ./FileCompressor

4. **Usage:** Provide local path targets to execute compression and decompression routines on raw files.
