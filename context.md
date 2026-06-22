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
  main.cpp             - Entry point + menu
  ...
test_cases/
  input_basic.txt, input_simple.txt, input_fcfs.txt,
  input_rr.txt, input_sjf.txt, input_srtn.txt
```

## Các bugs đã xác định (IMPROVEMENTS.md)

| Bug | Mô tả | Severity |
|-----|-------|----------|
| BUG-1 | Off-by-one CPU burst (processCPU trước pickNextCPUProcess) | 🔴 Cao |
| BUG-2 | SRTN so sánh currentBurst.remaining vs getRemainingCPUTotal() | 🔴 Cao |
| BUG-3 | markCompletion không đồng nhất (current_time vs +1) | 🟡 Trung |
| BUG-4 | Quantum RR +1 tick (do BUG-1) | 🔴 Cao |
| BUG-5 | Simulation termination thừa 1 tick idle | 🟡 Trung |
| BUG-8 | Memory leak khi writeOutput throw | 🟡 Trung |
