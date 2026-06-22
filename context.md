# Context: OS Process Scheduler Simulator

## Yêu cầu dự án

Viết chương trình C++ mô phỏng các thuật toán điều phối tiến trình (process scheduling).
Chương trình đọc bảng điều phối từ file đầu vào, mô phỏng và xuất biểu đồ Gantt ra file đầu ra dựa trên thuật toán được chỉ định.

## Tham số dòng lệnh

| Thứ tự | Bắt buộc | Mô tả |
|--------|----------|-------|
| 1 | ✓ | File đầu vào (`.txt`) |
| 2 | ✓ | File đầu ra (`.txt`) |
| 3 | ✓ | Thuật toán: 1=FCFS, 2=RR, 3=SJF, 4=SRTN |
| 4 | ✗ | Quantum cho RR (mặc định: 2) |

## Hành vi tiến trình & tài nguyên

- Mỗi tiến trình có thể sử dụng CPU và tài nguyên R nhiều lần, số lần khác nhau tùy ý.
- Hệ thống có **1 tài nguyên R**, điều phối theo **FCFS**.
- Khi có xung đột (nhiều tiến trình vào ready queue cùng lúc): **tiến trình mới được ưu tiên trước**.

## Định dạng file đầu vào

```
<dòng 1: số tiến trình N>
<dòng 2..N+1: arrival_time [cpu_burst r_burst]*>
```

Mỗi tiến trình: thời gian đến, sau đó là các cặp (CPU burst, R burst) lặp lại.

## Định dạng file đầu ra

- Dòng 1: Gantt CPU (số = PID, `_` = idle)
- Dòng 2: Gantt Resource R (số = PID, `_` = idle)

## Kiến trúc hiện tại

```
include/
  Process.hpp          - Tiến trình + TaskBurst (CPU/R)
  Parser.hpp           - Đọc/ghi file
  Resource.hpp         - Tài nguyên R (FCFS)
  ISchedulingAlgorithm.hpp - Interface Strategy
  Algorithms.hpp       - FCFS, RR, SJF, SRTN
  Scheduler.hpp        - Vòng lặp mô phỏng chính
  Visualizer.hpp       - TUI (ANSI colors)
  Analytics.hpp        - Báo cáo hiệu năng
src/
  main.cpp             - Entry point + menu (dùng unique_ptr)
  ...
test_cases/
  input_basic.txt, input_simple.txt, input_fcfs.txt,
  input_rr.txt, input_sjf.txt, input_srtn.txt,
  input_bug2_srtn.txt  (test cho BUG-2)
```

## Vòng lặp mô phỏng (sau tất cả fixes)

```
while true:
  if isSimulationFinished() → break
  handleNewArrivals
  processResource       (gantt/idle ghi trước decrement/start)
  resolveConflict
  checkSRTNPreemption   (so sánh total remaining CPU time)
  pickNextCPUProcess
  recordGantt           (ghi trạng thái CPU đầu tick)
  processCPU            (decrement burst)
  notifyVisualizer
  addWaitingTime
  ++current_time
```

## Các bugs đã fix

| Bug | Mô tả | Fix |
|-----|-------|-----|
| **BUG-1** | CPU off-by-one (processCPU trước pickNextCPUProcess) | Reorder loop, markCompletion(current_time+1) |
| **BUG-2** | SRTN so sánh currentBurst vs totalRemaining | `getRemainingCPUTotal()` cho cả 2 vế |
| **BUG-3** | markCompletion không đồng nhất | Cả CPU và Resource đều dùng `current_time + 1` |
| **BUG-4** | Quantum RR +1 tick | Tự động fix cùng BUG-1 |
| **BUG-5** | Thừa 1 tick idle cuối | Move isSimulationFinished lên đầu loop |
| **BUG-8** | Memory leak | `std::unique_ptr` thay `new`/`delete` |
| **BUG-R1** | Resource off-by-one (start không decrement, gantt sai tick cuối) | Ghi gantt/idle trước decrement/start |
