# ĐẶC TẢ YÊU CẦU ĐỒ ÁN: PHẦN MỀM MÔ PHỎNG LẬP LỊCH HỆ ĐIỀU HÀNH (C++)

## I. YÊU CẦU CHUNG (GENERAL REQUIREMENTS)
* 
**Báo cáo đồ án (.pdf):** Trình bày chi tiết cơ sở lý thuyết, sơ đồ thuật toán, cấu trúc thiết kế và kết quả thử nghiệm.


* 
**Mã nguồn (Source Code):** Toàn bộ các file thực thi `.cpp`, file cấu hình `.hpp` / `.h`, cùng file cấu hình biên dịch tự động (Makefile hoặc CMakeLists.txt).





---

## II. MÔ TẢ DỰ ÁN VÀ THAM SỐ DÒNG LỆNH

Sinh viên tiến hành xây dựng một chương trình bằng ngôn ngữ **C++** để mô phỏng lại cơ chế hoạt động của các thuật toán lập lịch tiến trình trong Hệ điều hành.

Chương trình sẽ tiếp nhận dữ liệu từ một tệp tin cấu hình đầu vào (Bảng lập lịch tiến trình), giả lập quá trình phân phối tài nguyên theo thời gian thực, và xuất kết quả sơ đồ biểu đồ Gantt ra một tệp tin đầu ra.

Chương trình phải hỗ trợ kích hoạt và truyền tham số trực tiếp từ giao diện dòng lệnh (Terminal) khi thực thi với cấu trúc bắt buộc như sau:

* 
**Tham số 1 (Bắt buộc):** Đường dẫn tới file dữ liệu đầu vào (Input filename).


* 
**Tham số 2 (Bắt buộc):** Đường dẫn tới file lưu kết quả đầu ra (Output filename).


* 
**Tham số 3 (Bắt buộc):** Mã số thuật toán lập lịch cho CPU, nhận giá trị số nguyên từ 1 đến 4 tương ứng với:


* 
`1`: **FCFS** (First-Come, First-Served).


* 
`2`: **RR** (Round Robin) – Áp dụng giá trị thời gian định mức (Quantum) mặc định bằng 2 nếu không có tham số thứ 4.


* 
`3`: **SJF** (Shortest Job First - Dạng không tiền định).


* 
`4`: **SRTN** (Shortest Remaining Time Next - Dạng tiền định).




* 
**Tham số 4 (Tùy chọn):** Giá trị giới hạn thời gian (Quantum) tự định nghĩa của người dùng khi sử dụng thuật toán RR.



---

## III. QUY TẮC MÔ PHỎNG HỆ THỐNG

Bộ giả lập phải vận hành dựa trên các nguyên lý quản lý tiến trình cốt lõi sau:

* 
**Chu kỳ hành vi đa tác vụ:** Mỗi tiến trình trong hệ thống có thể chiếm dụng CPU và Tài nguyên chia sẻ R nhiều lần xen kẽ nhau. Số lượng lượt sử dụng cũng như thời gian chiếm dụng (Burst time) của mỗi lượt là hoàn toàn bất định và thay đổi linh hoạt tùy theo từng tiến trình.


* 
**Độc lập điều phối tài nguyên:** Hệ thống chỉ trang bị duy nhất 1 thiết bị/tài nguyên chia sẻ R. Tài nguyên R này hoạt động độc lập với CPU và luôn luôn được điều phối cố định theo thuật toán **FCFS** (First-Come, First-Served).


* 
**Quy tắc giải quyết xung đột (Conflict Resolution):** Tại bất kỳ thời điểm $t$ nào trong quá trình mô phỏng, nếu xảy ra hiện tượng cạnh tranh vị trí xếp hàng ở lối vào của Hàng đợi sẵn sàng (Ready Queue) giữa một tiến trình hoàn toàn mới vừa đến hệ thống và một tiến trình cũ vừa thực thi xong một tác vụ ngoại vi (ví dụ: vừa dùng xong tài nguyên R và quay trở lại), **tiến trình mới (New process) luôn được ưu tiên xếp vào phía trước**.



---

## IV. ĐỊNH DẠNG FILE DỮ LIỆU CHUẨN

Tất cả các tệp tin lưu trữ đầu vào và đầu ra bắt buộc phải tuân theo định dạng văn bản phẳng mã hóa dạng `.txt`.

### 1. Định dạng File dữ liệu đầu vào (<INPUT_FILE>)

* Dòng đầu tiên ghi một số nguyên duy nhất chỉ định tổng số lượng tiến trình $N$ cần lập lịch.


* 
$N$ dòng tiếp theo mô tả chi tiết thông số của từng tiến trình tương ứng. Mỗi dòng là một chuỗi các số nguyên phân tách nhau bằng một dấu cách đơn, biểu diễn theo cấu trúc:
`<Thời gian đến> [<Thời gian chạy CPU> <Thời gian dùng R>]*` 



### 2. Định dạng File dữ liệu đầu ra (<OUTPUT_FILE>)

* 
**Dòng 1:** Chuỗi số nguyên biểu diễn biểu đồ Gantt của CPU qua từng giây. Sử dụng số thứ tự của tiến trình (ký tự `1` tương ứng với P1, `2` cho P2,...) và dùng ký tự gạch ngang `_` để biểu thị thời điểm CPU trống (Idle). Các ký tự cách nhau bằng một dấu cách.


* 
**Dòng 2:** Chuỗi ký tự tương tự dòng 1 nhằm biểu diễn tiến trình phân phối và chiếm dụng trên thiết bị/Tài nguyên chia sẻ R.



---

## V. YÊU CẦU MỞ RỘNG (EXTENDED REQUIREMENTS) - CHỨC NĂNG NÂNG CAO

> Nhằm tối ưu hóa điểm số và chứng minh năng lực thiết kế hệ thống chuyên nghiệp của nhóm 4 thành viên, chương trình C++ cần tích hợp thêm hai phân hệ mở rộng độc lập, giúp chuyển đổi ứng dụng từ dạng chạy ẩn (Silent CLI) thành một Dashboard quản trị trực quan.

### 1. Phân hệ Giao diện Động trên Terminal (Interactive TUI Visualizer)

Chương trình phải cung cấp thêm một chế độ hiển thị đồ họa trực quan ngay trên môi trường Console/Terminal bằng cách theo dõi trực tiếp từng đơn vị thời gian mô phỏng (từng giây thực tế).

* **Cơ chế hoạt động:** Khi kích hoạt, hệ thống sẽ tự động xóa sạch màn hình Terminal sau mỗi tích tắc thời gian và vẽ lại toàn bộ trạng thái hiện tại của hệ điều hành giả lập.
* **Các thành phần trực quan cần hiển thị:**
* **Bảng đồng hồ hệ thống:** Hiển thị thời gian thực tế $t$ đang chạy.
* **Trạng thái thực thi lõi:** Hiển thị rõ ràng tiến trình nào đang chiếm quyền kiểm soát CPU và tiến trình nào đang giữ Tài nguyên R, kèm theo thanh thời gian đếm ngược của lượt burst hiện tại.
* **Mô phỏng hàng đợi động:** Biểu diễn trực quan danh sách các tiến trình đang nằm trong Hàng đợi sẵn sàng (Ready Queue) của CPU và Hàng đợi chờ tài nguyên (Resource Queue) của R. Người xem phải thấy được thứ tự dịch chuyển, sự thay đổi độ ưu tiên của các tiến trình khi có xung đột xảy ra tại thời điểm $t$.
* **Đồ họa màu ANSI:** Sử dụng các dải màu ký tự tiêu chuẩn (như Xanh lá cho tiến trình đang chạy, Vàng cho tiến trình chờ, Đỏ cho hệ thống trống) để tăng tính công nghệ và dễ quan sát.



### 2. Phân hệ Phân tích và Đo đạc Hiệu năng (Performance Analytics Dashboard)

Bên cạnh việc xuất ra biểu đồ Gantt tĩnh, ngay khi vòng lặp mô phỏng kết thúc, chương trình bắt buộc phải tự động tổng hợp, tính toán và in ra một bảng báo cáo thống kê toán học về hiệu suất của các thuật toán. Các chỉ số chuyên sâu bao gồm:

* **Thời gian phản hồi (Response Time):** Khoảng thời gian từ lúc tiến trình bước vào hệ thống cho đến khi nó được CPU xử lý giây đầu tiên.
* **Thời gian chờ (Waiting Time):** Tổng thời gian tích lũy mà một tiến trình phải nằm chờ đợi trong Hàng đợi sẵn sàng.
* **Thời gian lưu lại hệ thống (Turnaround Time):** Tổng thời gian từ lúc tiến trình xuất hiện cho đến khi hoàn thành xong toàn bộ chuỗi tác vụ CPU và R để rời khỏi hệ thống.
* **Hiệu suất sử dụng CPU (CPU Utilization %):** Tỷ lệ phần trăm thời gian hoạt động thực tế có ích của CPU so với tổng thời gian mô phỏng (loại bỏ thời gian trống `_`).
* **Hiệu suất sử dụng Tài nguyên (Resource Utilization %):** Tỷ lệ phần trăm thời gian hoạt động thực tế của thiết bị R.

Bảng số liệu này sẽ đóng vai trò làm cơ sở dữ liệu để nhóm thực hiện phân tích, đánh giá ưu/nhược điểm của từng thuật toán trong file Báo cáo Đồ án (.pdf).