#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "Process.hpp"
#include <queue>
#include <vector>

class Resource {
public:
    Process* current_user; // Tiến trình đang sử dụng tài nguyên
    int remaining_time; // Thời gian còn lại mà tiến trình đang sử dụng tài nguyên cần để hoàn thành burst RESOURCE hiện tại
    bool is_busy; // Trạng thái của tài nguyên, true nếu đang bận, false nếu rảnh
    std::queue<Process*> res_queue; // Hàng đợi các tiến trình đang chờ sử dụng tài nguyên
    std::queue<Process*> released_queue; // Hàng đợi các tiến trình đã hoàn thành burst RESOURCE và sẽ được chuyển lại vào hàng đợi sẵn sàng
    std::vector<int> gantt_history; //  Lịch sử sử dụng tài nguyên theo thời gian, lưu trữ id của tiến trình đang sử dụng tài nguyên tại mỗi đơn vị thời gian, hoặc -1 nếu tài nguyên rảnh
    int total_idle_time; // Tổng thời gian tài nguyên rảnh trong suốt quá trình mô phỏng

    Resource(); // Khởi tạo tài nguyên ở trạng thái rảnh

    void addRequest(Process* p);
    void tick();
    Process* popReleased();
    int getQueueSize() const;
    std::queue<Process*> getQueueSnapshot() const;
};

#endif
