# Hướng dẫn sử dụng OS Scheduler Simulator

## 1. Build

```bash
cd OS_Scheduler_Project
make
```

Yêu cầu: `g++` hỗ trợ C++17.

---

## 2. Chế độ tương tác (Menu)

Chạy **không tham số** để vào menu:

```bash
./scheduler
```

Menu sẽ hiện ra:

```
╔═══════════════════════════════════════╗
║  OS SCHEDULER SIMULATOR              ║
║  Interactive Mode                    ║
╚═══════════════════════════════════════╝

1) FCFS
2) Round Robin
3) SJF
4) SRTN
Chon thuat toan [1]:
File input [test_cases/input_basic.txt]:
File output [output/gantt.txt]:
Quantum [2]:
Bat TUI? (y/N):
```

Nhấn **Enter** để chọn giá trị mặc định (trong `[ ]`).

---

## 3. Chế độ dòng lệnh (CLI)

```bash
./scheduler <input_file> <output_file> <algorithm_code> [quantum] [--tui]
```

### Tham số

| Tham số | Vị trí | Bắt buộc | Mô tả |
|---------|--------|----------|-------|
| `input_file` | 1 | ✓ | Đường dẫn file input `.txt` |
| `output_file` | 2 | ✓ | Đường dẫn file output `.txt` (Gantt chart) |
| `algorithm_code` | 3 | ✓ | `1`=FCFS, `2`=RR, `3`=SJF, `4`=SRTN |
| `quantum` | 4 | ✗ | Quantum cho RR (mặc định: `2`) |
| `--tui` | 5 | ✗ | Bật giao diện trực quan trên terminal |

### Ví dụ

```bash
# FCFS
./scheduler test_cases/input_basic.txt output/gantt.txt 1

# RR với quantum=4
./scheduler test_cases/input_basic.txt output/gantt.txt 2 4

# RR với quantum mặc định (2)
./scheduler test_cases/input_basic.txt output/gantt.txt 2

# SJF
./scheduler test_cases/input_basic.txt output/gantt.txt 3

# SRTN
./scheduler test_cases/input_basic.txt output/gantt.txt 4

# RR + giao diện TUI
./scheduler test_cases/input_basic.txt output/gantt.txt 2 4 --tui
```

---

## 4. Định dạng file input

```
<dòng 1: số lượng tiến trình N>
<dòng 2..N+1: arrival_time cpu_burst r_burst cpu_burst r_burst ...>
```

### Ví dụ (`test_cases/input_basic.txt`):

```
3
0 5 3 2 2
2 4 2
5 3 1 2 1
```

| Process | Arrival | Bursts |
|---------|---------|--------|
| P1 | t=0 | CPU 5s → R 3s → CPU 2s → R 2s |
| P2 | t=2 | CPU 4s → R 2s |
| P3 | t=5 | CPU 3s → R 1s → CPU 2s → R 1s |

Mỗi dòng: thời gian đến, sau đó là các cặp `(CPU burst, R burst)` lặp lại.

---

## 5. Định dạng file output

**Dòng 1:** Biểu đồ Gantt CPU (số = process ID, `_` = idle)  
**Dòng 2:** Biểu đồ Gantt tài nguyên R

```
1 1 1 1 1 2 2 2 2 3 3 3 1 1 3 3 _ _
_ _ _ _ _ 1 1 1 _ 2 2 _ 3 _ 1 1 3 _
```

---

## 6. Đầu ra báo cáo Analytics

Sau khi chạy, chương trình tự động in bảng thống kê hiệu năng:

```
╔══════════════════════ PERFORMANCE ANALYTICS ══════════════════╗
║  Algorithm: FCFS                                              ║
║  Total Simulation Time: 18s                                   ║
║                                                               ║
║  ┌─────┬──────────┬──────────┬──────────────┐                ║
║  │ PID │ Response │ Waiting  │ Turnaround   │                ║
║  ├─────┼──────────┼──────────┼──────────────┤                ║
║  │ P1  │     0.00 │     4.00 │        17.00 │                ║
║  │ P2  │     3.00 │     3.00 │        10.00 │                ║
║  │ P3  │     4.00 │     5.00 │        13.00 │                ║
║  ├─────┼──────────┼──────────┼──────────────┤                ║
║  │ Avg │     2.33 │     4.00 │        13.33 │                ║
║  └─────┴──────────┴──────────┴──────────────┘                ║
║                                                               ║
║  ┌──────────────────────────────────────┐                    ║
║  │ CPU Utilization:    88.88%           │                    ║
║  │ Resource Utilization: 50.00%         │                    ║
║  └──────────────────────────────────────┘                    ║
╚═══════════════════════════════════════════════════════════════╝
```

### Các chỉ số

| Chỉ số | Ý nghĩa |
|--------|---------|
| **Response Time** | Thời gian từ lúc đến → lần đầu CPU xử lý |
| **Waiting Time** | Tổng thời gian chờ trong Ready Queue |
| **Turnaround Time** | Thời gian từ lúc đến → hoàn thành toàn bộ |
| **CPU Utilization** | % thời gian CPU hoạt động (không idle) |
| **Resource Utilization** | % thời gian tài nguyên R hoạt động |

---

## 7. Tình huống mô phỏng (thuật toán)

| Thuật toán | Mã | Đặc điểm |
|-----------|-----|---------|
| **FCFS** | `1` | Non-preemptive, chọn process đến trước |
| **RR** | `2` | Preemptive theo quantum, mặc định `q=2` |
| **SJF** | `3` | Non-preemptive, chọn CPU burst ngắn nhất |
| **SRTN** | `4` | Preemptive, chọn remaining time ngắn nhất |

### Quy tắc mô phỏng
- **Resource R** hoạt động độc lập với CPU, điều phối theo **FCFS**
- Khi có **process mới đến** và **process từ R quay về** cùng lúc → **process mới được ưu tiên** xếp trước trong Ready Queue

---

## 8. Các file test có sẵn

| File | Mô tả |
|------|-------|
| `test_cases/input_basic.txt` | 3 process, burst CPU/R xen kẽ |
| `test_cases/input_simple.txt` | 2 process, chỉ CPU, không có R |
| `test_cases/input_fcfs.txt` | Test cho FCFS |
| `test_cases/input_rr.txt` | Test cho RR (cùng arrival time) |
| `test_cases/input_sjf.txt` | Test cho SJF |
| `test_cases/input_srtn.txt` | Test cho SRTN |

---

## 9. Kiến trúc project

```
OS_Scheduler_Project/
├── Makefile                # Build script
├── USAGE.md                # Hướng dẫn sử dụng
│
├── include/                # Header files (.hpp)
│   ├── Process.hpp         # Cấu trúc tiến trình và TaskBurst
│   ├── Parser.hpp          # Đọc/ghi file input/output
│   ├── Resource.hpp        # Quản lý tài nguyên R
│   ├── ISchedulingAlgorithm.hpp  # Interface Strategy Pattern
│   ├── Algorithms.hpp      # 4 thuật toán lập lịch (FCFS, RR, SJF, SRTN)
│   ├── Scheduler.hpp       # Bộ điều phối trung tâm (vòng lặp mô phỏng)
│   ├── Visualizer.hpp      # Giao diện TUI (ANSI colors)
│   └── Analytics.hpp       # Báo cáo hiệu năng
│
├── src/                    # Source files (.cpp)
│   ├── main.cpp            # Entry point + interactive menu
│   ├── Process.cpp
│   ├── Parser.cpp
│   ├── Resource.cpp
│   ├── ISchedulingAlgorithm.cpp
│   ├── Algorithms.cpp
│   ├── Scheduler.cpp
│   ├── Visualizer.cpp
│   └── Analytics.cpp
│
├── test_cases/             # File input mẫu (.txt)
└── output/                 # File kết quả Gantt (.txt)
```
