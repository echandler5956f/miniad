# miniad

*Header-only C++20 linear-algebra **and** automatic-differentiation library built as a public, test-driven tutorial.*

> **Why exist?**  
> We want a minimal Eigen-style expression-template engine that also provides
> forward **and** reverse automatic differentiation—all developed live, phase
> by phase, with production-quality tests and tooling.  
> Full curriculum: [`docs/Tutorial-Plan.md`](docs/Tutorial-Plan.md).

---

## Quick-start (Phase 0)

```bash
git clone https://github.com/echandler5956f/miniad.git
cd miniad
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target miniad_tests
ctest --test-dir build --output-on-failure
````

> Requires **CMake ≥ 3.13** and a C++20-capable compiler (GCC 11 / Clang 10).

---

## Example (will evolve)

```cpp
#include <miniad/scalar.hpp>

int main() {
    miniad::scalar<double> x = 3.0;        // value = 3, dual = 1
    miniad::scalar<double> y = 4.0;        // value = 4, dual = 0
    auto z = x + y;
    std::cout << z.value() << '\n';        // 7.0
}
```

This snippet will compile and run after **Phase 1**.

---

## Documentation & Progress

* Curriculum: [`docs/Tutorial-Plan.md`](docs/Tutorial-Plan.md)
* Live progress log: [`docs/Progress-Log.md`](docs/Progress-Log.md)
* Phase checklist: [`docs/Phase-Checklist.md`](docs/Phase-Checklist.md)

---

*© 2025 Ethan Chandler — MIT License (to be finalised).*
