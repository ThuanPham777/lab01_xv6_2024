#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2];  // pipe 1: cha ghi - con đọc
    int p2[2];  // pipe 2: con ghi - cha đọc
    char buf[10];

    // Tạo hai pipe: một để tiến trình cha gửi "ping", một để con gửi "pong"
    pipe(p1);
    pipe(p2);

    int pid = fork();  // Tạo tiến trình con
    if (pid < 0) {
        // Nếu fork thất bại
        printf("error\n");
        exit(1);
    } else if (pid == 0) {
        // Tiến trình con

        // Đóng các đầu không cần thiết
        close(p1[1]);  // Đóng đầu ghi của pipe 1 (chỉ dùng để đọc)
        close(p2[0]);  // Đóng đầu đọc của pipe 2 (chỉ dùng để ghi)

        // Đọc từ pipe 1 (dữ liệu từ cha)
        read(p1[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);  // In ra dữ liệu nhận được từ cha

        // Ghi "pong" vào pipe 2 để gửi lại cha
        write(p2[1], "pong", 5);

        // Đóng các đầu còn lại
        close(p1[0]);
        close(p2[1]);
        exit(0);
    } else {
        // Tiến trình cha

        // Đóng các đầu không cần thiết
        close(p1[0]);  // Đóng đầu đọc của pipe 1 (chỉ dùng để ghi)
        close(p2[1]);  // Đóng đầu ghi của pipe 2 (chỉ dùng để đọc)

        // Ghi "ping" vào pipe 1
        write(p1[1], "ping", 5);

        // Chờ tiến trình con kết thúc
        wait(0);

        // Đọc từ pipe 2 (dữ liệu từ con)
        read(p2[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);  // In ra dữ liệu nhận được từ con

        // Đóng các đầu còn lại
        close(p1[1]);
        close(p2[0]);
    }

    exit(0);
}
