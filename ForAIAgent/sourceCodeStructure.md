## CẤU TRÚC ĐỀ XUẤT CẢI TIẾN (V2)

Để đáp ứng trọn vẹn cả các quy tắc mô phỏng cốt lõi và các phân hệ mở rộng nâng cao (TUI trực quan và Dashboard phân tích), mã nguồn C++ của dự án được thiết kế hướng đối tượng (OOP) theo mô hình Modular. Việc tách biệt giữa **Dữ liệu lõi (Core)**, **Giao diện (UI)** và **Phân tích (Analytics)** giúp nhóm 4 người có thể lập trình song song mà không bị xung đột code.

### 1. Sơ đồ cây thư mục tổng thể

```text
OS_Scheduler_Project/
│
├── CMakeLists.txt                  # File cấu hình build dự án tự động bằng CMake (chọn 1, khuyến nghị)
│
├── include/                        # Thư mục chứa các tệp tin khai báo (.hpp)
│   ├── Process.hpp                 # Mô hình hóa Tiến trình và chuỗi TaskBurst (CPU/R xen kẽ)
│   ├── ISchedulingAlgorithm.hpp    # Interface Strategy Pattern cho thuật toán CPU
│   ├── Scheduler.hpp               # Bộ khung điều phối trung tâm (giữ vòng lặp mô phỏng)
│   ├── Algorithms.hpp              # 4 lớp cài đặt cụ thể (FCFS, RR, SJF, SRTN) implement interface
│   ├── Resource.hpp                # Module quản lý tài nguyên R (hàng đợi riêng, FCFS)
│   ├── Parser.hpp                  # Đọc/ghi file input/output (parse định dạng burst biến động)
│   ├── Visualizer.hpp              # Phân hệ Giao diện Động trên Terminal (Interactive TUI)
│   └── Analytics.hpp               # Phân hệ Đo đạc Hiệu năng (Performance Analytics Dashboard)
│
├── src/                            # Thư mục chứa mã nguồn triển khai chi tiết (.cpp)
│   ├── main.cpp                    # Điểm khởi chạy, xử lý argv[], khởi tạo đối tượng
│   ├── Process.cpp                 # Định nghĩa Process + TaskBurst
│   ├── Scheduler.cpp               # Vòng lặp mô phỏng, Observer cho Visualizer
│   ├── Algorithms.cpp              # Cài đặt 4 thuật toán: FCFS, RR, SJF, SRTN
│   ├── Resource.cpp                # Logic quản lý hàng đợi và cấp phát tài nguyên R
│   ├── Parser.cpp                  # Parse input, xuất output Gantt Chart
│   ├── Visualizer.cpp              # TUI: xóa/vẽ màn hình, màu ANSI, cập nhật real-time
│   └── Analytics.cpp               # Tính Response, Waiting, Turnaround, Utilization %
│
├── test_cases/                     # Thư mục chứa các kịch bản kiểm thử đầu vào (.txt)
│   ├── input_basic.txt
│   ├── input_fcfs.txt
│   ├── input_rr.txt
│   ├── input_sjf.txt
│   └── input_srtn.txt
│
└── output/                         # Thư mục lưu trữ tự động các file kết quả Gantt Chart
    └── gantt_result.txt

```

---

### 2. Mô tả chi tiết vai trò của các Module thành phần

### `include/` & `src/` (Thành phần xử lý Logic và Giao diện)

* **Process (Thực thể Tiến trình):**
    * Định nghĩa cấu trúc dữ liệu mô tả một tiến trình bao gồm: ID, `arrival_time`, và một danh sách mảng các **TaskBurst** (mỗi burst có: loại `CPU|R`, `duration`, `remaining_time`).
    * Lưu trữ các biến trạng thái phục vụ đo đạc hiệu năng: `first_response_time`, `completion_time`, `total_waiting_time`.

* **ISchedulingAlgorithm (Interface - Strategy Pattern):**
    * Định nghĩa interface thuần ảo `virtual int pickNext(const vector<Process>&) = 0;`
    * Cho phép `Scheduler` giữ vòng lặp duy nhất, chỉ thay đổi **chiến lược chọn process** qua con trỏ interface → tránh trùng lặp code.

* **Scheduler (Bộ khung điều phối trung tâm):**
    * Là "Trái tim" hệ thống, chứa **vòng lặp mô phỏng duy nhất** (chạy từng giây).
    * Quản lý **Ready Queue** (CPU) và giao tiếp với `Resource` module cho **Resource Queue** (R).
    * Chứa logic **giải quyết xung đột**: tại cùng thời điểm $t$, new process được ưu tiên đứng trước process từ R trở về.
    * Sử dụng **Observer Pattern**: thông báo (`notify()`) cho `Visualizer` mỗi tick.
    * Gọi `ISchedulingAlgorithm::pickNext()` khi cần chọn process cho CPU.

* **Algorithms (Các thuật toán lập lịch cụ thể):**
    * Cài đặt interface `ISchedulingAlgorithm`:
        * `FCFSScheduler` – chọn process đến sớm nhất.
        * `RoundRobinScheduler` – chọn theo vòng, xử lý quantum.
        * `SJFScheduler` – chọn process có tổng CPU burst ngắn nhất (non-preemptive).
        * `SRTNScheduler` – kiểm tra mỗi giây, chọn process có remaining time ngắn nhất (preemptive).

* **Resource (Module quản lý tài nguyên R):**
    * Module độc lập, quản lý riêng **Resource Queue** FCFS.
    * Xử lý cấp phát và giải phóng tài nguyên R xen kẽ với CPU.

* **Parser (Module đọc/ghi dữ liệu):**
    * **Input:** Parse file `.txt` với định dạng `<arrival_time> [<CPU_burst> <R_burst>]*` (số burst thay đổi linh hoạt theo từng process).
    * **Output:** Ghi biểu đồ Gantt (CPU dòng 1, R dòng 2) theo format `1 2 _ ...`.

* **Visualizer (Phân hệ mở rộng TUI):**
    * Đăng ký nhận sự kiện mỗi tick từ `Scheduler` (Observer Pattern).
    * Xóa màn hình, vẽ lại real-time: đồng hồ hệ thống, CPU/R status (kèm countdown), Ready Queue & Resource Queue động, ANSI color codes (Xanh=running, Vàng=waiting, Đỏ=idle).

* **Analytics (Phân hệ mở rộng Dashboard):**
    * Sau khi vòng lặp mô phỏng kết thúc, quét toàn bộ dữ liệu lịch sử.
    * Tính toán: **Response Time**, **Waiting Time**, **Turnaround Time**, **CPU Utilization %**, **Resource Utilization %**.
    * In bảng báo cáo thống kê ra terminal.

* **main.cpp (Điểm kích hoạt):**
    * Bóc tách `argv[]` → kiểm tra tính hợp lệ (file, mã thuật toán, quantum).
    * `Parser` đọc input → khởi tạo mảng `Process`.
    * `Scheduler` + `ISchedulingAlgorithm` tương ứng (dùng Factory hoặc switch).
    * Kích hoạt vòng lặp mô phỏng → xuất output qua `Parser`, gọi `Analytics`.