# OS Scheduler Simulator — Improvements & Roadmap

## 🐛 Bugs (HIGH)

### ~~BUG-1: Off-by-one CPU burst accounting~~ ✅ FIXED
**File:** `src/Scheduler.cpp:30-71, 85-120`

**Fix:** Reorder simulation loop: `processCPU()` moved after `recordGantt()` so burst is decremented from tick 0. Also fixed `markCompletion(current_time)` → `current_time + 1` for consistency.

---

### ~~BUG-2: SRTN preemption so sánh sai đại lượng~~ ✅ FIXED
**File:** `src/Scheduler.cpp:146-159`

**Fix:** Changed `getCurrentBurst().remaining` → `getRemainingCPUTotal()` so preemption compares total remaining CPU time, not just current burst.

---

### ~~BUG-3: markCompletion không đồng nhất~~ ✅ FIXED
**File:** `src/Scheduler.cpp:100, 127`

**Fix:** Both CPU and Resource paths now use `markCompletion(current_time + 1)`.

---

### ~~BUG-4: Quantum RR bị +1 tick~~ ✅ FIXED
**File:** `src/Scheduler.cpp:91-93, 115-119, 174-175`

**Note:** Side effect of BUG-1. Automatically resolved when loop was reordered.

---

### BUG-5: Simulation termination thừa 1 tick idle
**File:** `src/Scheduler.cpp:64-73, 211-222`

**Status:** ✅ FIXED

**Vấn đề:** `isSimulationFinished()` được kiểm tra *sau* `recordGantt()`. Khi tick cuối mọi process kết thúc, idle tick tiếp theo vẫn được ghi vào Gantt trước khi loop break.

**Fix:** Chuyển `isSimulationFinished()` lên đầu loop, `total_simulation_time = current_time` (bỏ `+1`).

---

### BUG-8: Memory leak trên error path
**File:** `src/main.cpp:144-150, 162`

**Status:** ✅ FIXED

**Vấn đề:** `algo` được `new` nhưng nếu `Parser::writeOutput()` throw, `delete algo` bị skip.

**Fix:** Dùng `std::unique_ptr<ISchedulingAlgorithm>` thay `new`/`delete`.

---

### BUG-R1: Resource off-by-one (phát hiện mới)
**File:** `src/Resource.cpp:14-40`

**Status:** ✅ FIXED

**Vấn đề:** Resource có 2 lỗi giống BUG-1 cũ:
1. Process mới lên R không được decrement trong tick đầu (start nhưng không chạy)
2. Khi process kết thúc trên R, gantt ghi idle thay vì ghi process (vì gantt push sau khi `current_user` đã set null)

**Fix:** Chuyển `gantt_history.push_back()` và `++total_idle_time` lên trước block xử lý decrement và start, ghi nhận trạng thái đầu tick.

---

## 🏗️ Missing Features (MEDIUM)

### MF-1: Thêm thuật toán MLFQ (Multi-Level Feedback Queue)
**Mô tả:** Thuật chuẩn trong OS hiện đại (BSD, Solaris). Cho phép process di chuyển giữa các priority queue dựa trên hành vi CPU-bound vs I/O-bound.

**Gợi ý:** Cấu hình số lượng queue, quantum mỗi queue, và boost interval.

---

### MF-2: Thêm Priority Scheduling
**Mô tả:** Preemptive/non-preemptive priority scheduling. Cần thêm trường `priority` vào input format.

---

### MF-3: Thêm `--batch` mode
**Mô tả:** Chạy nhiều test case tự động, xuất kết quả dạng CSV/JSON để so sánh các thuật toán.

```
./scheduler --batch test_list.txt --csv results.csv
```

---

### MF-4: Thêm `--csv` / `--json` export
**Mô tả:** Export analytics ra structured format thay vì chỉ print terminal.

---

### MF-5: TUI step mode
**Mô tả:** Cho phép người dùng bấm phím để step từng tick, thay vì auto-play 100ms.

---

### MF-6: Process state tracking
**Mô tả:** Track trạng thái (NEW, READY, RUNNING, BLOCKED, TERMINATED) và hiển thị trong TUI.

---

### MF-7: Starvation detection
**Mô tả:** Cảnh báo nếu SJF/SRTN gây starvation cho process nào (waiting time > threshold).

---

### MF-8: Context-switch overhead
**Mô tả:** Thêm tham số context-switch cost (ms) vào simulation để realistic hơn.

---

## 🧹 Code Quality (LOW)

### CQ-1: Dùng `unique_ptr` thay `new`/`delete`
**File:** `src/main.cpp`

**Status:** ✅ FIXED (cùng BUG-8)

### CQ-2: Encapsulation
**File:** `include/*.hpp`

Hầu hết member variables là `public`. Cần chuyển thành `private` + accessor methods.

### CQ-3: `Analytics` copy vector processes
**File:** `Analytics.hpp:25`, `Analytics.cpp:7-23`

Nên dùng `const std::vector<Process>&` (reference) thay vì copy by value.

### CQ-4: Remove `#include "Algorithms.hpp"` khỏi `Scheduler.hpp`
**File:** `Scheduler.hpp:7`

Không cần thiết, chỉ cần `ISchedulingAlgorithm` forward declaration.

### CQ-5: Xóa dead code `time_entered_ready_queue`
**File:** `Process.cpp:51-53`, `Process.hpp`

Field này bị overwrite mỗi lần vào queue, không được dùng ở đâu.

### CQ-6: Validate input burst > 0
**File:** `Parser.cpp`

Parser không kiểm tra burst duration âm hoặc bằng 0.

### CQ-7: SRTN và SJF dùng chung logic pickNext
**File:** `Algorithms.cpp:20-47`

Hai hàm `pickNext` giống hệt nhau. Có thể share base class.

### CQ-8: Auto-create output directory
**File:** `Parser.cpp:60`

Dùng `std::filesystem::create_directories` trước khi mở file output.

### CQ-9: `isatty` check cho ANSI codes
**File:** `Visualizer.cpp`, `Analytics.cpp`

Khi pipe output vào file, ANSI codes hiện ra raw text. Cần kiểm tra `isatty(STDOUT_FILENO)`.

### CQ-10: Thêm `--help` flag
**File:** `main.cpp`

Hiện tại không xử lý `--help` hay `-h`.

### CQ-11: Magic string ANSI codes → constants
**File:** `Visualizer.cpp`, `Analytics.cpp`

Định nghĩa `enum class Color` hoặc hằng số thay vì `\033[31m` rải rác.

---

## ⚡ Performance

Không có vấn đề performance nghiêm trọng. Có thể optimize phần `Visualizer::render()` nếu simulation rất dài (>10k ticks) nhưng không cần thiết cho mục đích học thuật.
