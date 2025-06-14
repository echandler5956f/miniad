# miniad – Progress Log
> Single source of truth for where the interactive tutorial currently stands.

| Date (UTC) | Phase | Summary of work completed | Next action |
|------------|-------|---------------------------|-------------|
| _n/a_      | 0     | _bootstrap not started_   | Create directory skeleton |
| 2025-06-10 | 0 | Bootstrapped project skeleton and added passing trivial test | Write failing test for scalar<T> |
| 2025-06-10 | 1 | Added failing test for scalar<T> forward dual arithmetic | Implement scalar<T> to pass tests |
| 2025-06-10 | 1 | Implemented basic arithmetic, unary minus, sqrt, and exp for scalar<T> (commit 37079ad) | Add tests for edge cases (e.g., sqrt(0)) |
| 2025-06-10 | 1 | Completed Phase 1 with edge-case tests for scalar<T> (commit 540f092) | Introduce Expression Templates and begin Phase 2 |
| 2025-06-10 | 2 | Introduced expr_base for CRTP-based expression templates (commit 575fe23) | Refactor scalar<T> to be an expression template |
| 2025-06-10 | 2 | Refactored binary operators (+,-,*,/) into the expression template system (commit 7a32112) | Re-implement unary operators (negation, sqrt, exp) as expression templates |
| 2025-06-10 | 2 | Completed Phase 2 with full ET system for unary and binary ops (commit 9358b8a) | Conclude Phase 2 and begin Phase 3 |
| 2025-06-10 | 3 | Implemented matrix facade and element access (commit bb48fde) | Add matrix addition expression template |
