# OS Scheduler Simulator — Improvements & Roadmap

## 🐛 Bugs (HIGH)

### BUG-1: Off-by-one CPU burst accounting
**File:** `src/Scheduler.cpp:30-71, 85-120`

**Vấn đề:** Trong `runSimulation()`, `processCPU()` được gọi *trước* `pickNextCPUProcess()`. Ở tick đầu tiên (`t=0`), chưa có process nào trên CPU → `processCPU()` không làm gì. Sau đó `pickNextCPUProcess()` chọn process và gán vào CPU, nhưng burst không được decrement cho tick đó. Process đầu tiên luôn nhận +1 tick CPU so với burst được cấu hình.

**Hậu quả:** Mọi process bị sai số liệu CPU time +1.

**Fix:** Chuyển `processCPU()` xuống *sau* `pickNextCPUProcess()` trong loop, hoặc restructure loop.

---

### BUG-2: SRTN preemption so sánh sai đại lượng
**File:** `src/Scheduler.cpp:146-159`

```cpp
int current_remaining = cpu_current->getCurrentBurst().remaining; // chỉ burst hiện tại
// so sánh với:
p->getRemainingCPUTotal() // tổng tất cả CPU burst còn lại
```

**Vấn đề:** So sánh `remaining của 1 burst` với `tổng remaining của nhiều burst` — không cùng đơn vị.

**Fix:** Đổi `cpu_current->getCurrentBurst().remaining` thành `cpu_current->getRemainingCPUTotal()`.

---

### BUG-3: markCompletion không đồng nhất
**File:** `src/Scheduler.cpp:100, 127`

| Đường dẫn | `markCompletion` |
|-----------|-----------------|
| Kết thúc bằng CPU burst | `markCompletion(current_time)` |
| Kết thúc bằng Resource burst | `markCompletion(current_time + 1)` |

**Fix:** Thống nhất convention — khuyến nghị dùng `current_time` cho cả hai.

---

### BUG-4: Quantum RR bị +1 tick
**File:** `src/Scheduler.cpp:91-93, 115-119, 174-175`

**Nguyên nhân:** Hậu quả của BUG-1. Quantum counter được reset trong `pickNextCPUProcess()` nhưng tick đầu tiên không chạy `processCPU()` → process mới hưởng 1 tick free trước khi counter bắt đầu đếm. Với `quantum=2`, process chạy 3 ticks thay vì 2.

---

### BUG-5: Simulation termination thừa 1 tick idle
**File:** `src/Scheduler.cpp:65-73, 211-222`

**Vấn đề:** `isSimulationFinished()` được kiểm tra *sau* record Gantt nhưng *trước* `++current_time`. Nếu tick cuối làm simulation finished, `++current_time` vẫn chạy → `total_simulation_time` nhiều hơn số tick thực tế 1 đơn vị, thêm 1 tick idle vào Gantt.

---

### BUG-8: Memory leak trên error path
**File:** `src/main.cpp:144-150, 162`

**Vấn đề:** `algo` được `new` nhưng nếu `Parser::writeOutput()` throw, `delete algo` bị skip.

**Fix:** Dùng `std::unique_ptr<ISchedulingAlgorithm>`.

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

---

## 📋 Ưu tiên đề xuất

| Thứ tự | Item | Effort | Impact |
|--------|------|--------|--------|
| 1 | BUG-1 + BUG-4 (off-by-one burst/quantum) | 1-2h | 🔴 Cao |
| 2 | BUG-2 (SRTN sai comparison) | 30m | 🔴 Cao |
| 3 | BUG-3 (markCompletion không đồng nhất) | 15m | 🟡 Trung bình |
| 4 | BUG-8 (memory leak) | 15m | 🟡 Trung bình |
| 5 | Thêm MLFQ | 4-6h | 🟢 Thấp |
| 6 | Thêm `--batch` + CSV export | 2-3h | 🟢 Thấp |
| 7 | TUI step mode | 1-2h | 🟢 Thấp |
| 8 | Code quality (encapsulation, smart pointers, ...) | 3-4h | 🟢 Thấp |
