#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>
#include <cstddef>

enum class BurstType { CPU, RESOURCE }; // Định nghĩa kiểu enum để phân biệt giữa các loại burst: CPU và RESOURCE

struct TaskBurst {
    BurstType type;
    int duration;
    int remaining;

    TaskBurst(BurstType t, int d)
        : type(t), duration(d), remaining(d) {}
};

class Process {
public:
    int id;
    int arrival_time;
    std::vector<TaskBurst> bursts; // Danh sách các burst của tiến trình, mỗi burst có thể là CPU hoặc RESOURCE
    size_t current_burst_idx; // Chỉ số của burst hiện tại mà tiến trình đang thực hiện
    int first_response_time;// Thời điểm tiến trình được CPU xử lý lần đầu tiên
    int completion_time; // Thời điểm tiến trình hoàn thành
    int total_waiting_time; // Tổng thời gian chờ của tiến trình
    int time_entered_ready_queue; // Thời điểm tiến trình vào hàng đợi sẵn sàng gần nhất

    Process(int _id, int _arrival);

    bool isFinished() const;
    TaskBurst& getCurrentBurst(); // Phương thức để lấy burst hiện tại của tiến trình
    void advanceBurst(); // Phương thức để chuyển sang burst tiếp theo
    int getRemainingCPUTotal() const; // Phương thức để lấy tổng thời gian CPU còn lại
    void markFirstResponse(int time); // Phương thức để đánh dấu thời điểm tiến trình được CPU xử lý lần đầu tiên
    void markCompletion(int time); // Phương thức để đánh dấu thời điểm tiến trình hoàn thành
    void addWaitingTime(int additional_time /*additional_time là thời gian chờ cần thêm */); // Phương thức để thêm thời gian chờ vào tiến trình
    void enterReadyQueue(int current_time); // Phương thức để đánh dấu thời điểm tiến trình vào hàng đợi sẵn sàng gần nhất,
    // dùng để tính thời gian chờ khi tiến trình được chọn lại sau khi bị preempted
};

#endif
