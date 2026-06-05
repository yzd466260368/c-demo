// #include <stdio.h>
// #include <string.h>

// enum Role
// {
//     ADMIN,
//     USER,
//     GUEST
// };

// struct User
// {
//     int id;
//     enum Role role;
// };

// void printUser(struct User user)
// {
//     char roleStr[10];

//     switch (user.role)
//     {
//     case ADMIN:
//         strcpy(roleStr, "管理员");
//         break;
//     case USER:
//         strcpy(roleStr, "用户");
//         break;
//     case GUEST:
//         strcpy(roleStr, "游客");
//         break;
//     default:
//         break;
//     }

//     printf("用户id是：%d的角色类型是：%s\n", user.id, roleStr);
// }

// int main(int argc, char const *argv[])
// {
//     /*
//     定义一个 Role 枚举类型，表示身份类型：ADMIN、USER 和 GUEST。
//     定义一个 User 结构体，包含：
//     o 用户ID id；
//     o 角色 Role。
//     在主函数中：
//     o 创建三个 User 实例，初始化不同角色；
//     o 根据 Role 输出每个用户的权限信息。
//     提示：使用 switch 判断不同角色并输出相应权限信息。
//     */

//     struct User user1 = {1, ADMIN};
//     struct User user2 = {2, USER};
//     struct User user3 = {3, GUEST};

//     printUser(user1);
//     printUser(user2);
//     printUser(user3);

//     return 0;
// }

// #include <stdio.h>
// #include <string.h>

// enum DeviceState
// {
//     ON,
//     OFF,
//     SLEEP
// };
// struct Device
// {
//     int id;
//     char name[21];
//     enum DeviceState state;
// };

// void printDeviceState(struct Device device)
// {
//     printf("设备ID：%d\n", device.id);
//     printf("设备名称：%s\n", device.name);

//     switch (device.state)
//     {
//     case ON:
//         printf("设备状态：开启\n");
//         break;

//     case OFF:
//         printf("设备状态：关闭\n");
//         break;

//     case SLEEP:
//         printf("设备状态：睡眠\n");
//         break;

//     default:
//         printf("设备状态：未知\n");
//         break;
//     }

//     printf("--------------------\n");
// }

// int main(int argc, char const *argv[])
// {
//     /*
//     定义一个 DeviceState 枚举类型：ON、OFF 和 SLEEP。
//     定义一个 Device 结构体，包含：
//     设备ID id；
//     名称 name（字符串，长度不超过20）；
//     状态 DeviceState。
//     在主函数中：
//     创建并初始化两个 Device 变量；
//     改变设备状态并输出设备状态。
//     提示：使用 switch 判断设备状态并输出描述性信息。
//     */

//     struct Device device1 = {1, "Computer", OFF};
//     struct Device device2 = {2, "Printer", ON};

//     printf("初始设备状态：\n");
//     printDeviceState(device1);
//     printDeviceState(device2);

//     // 改变设备状态
//     device1.state = ON;
//     device2.state = SLEEP;

//     printf("改变后的设备状态：\n");
//     printDeviceState(device1);
//     printDeviceState(device2);

//     return 0;
// }

// #include <stdio.h>
// #include <string.h>

// enum Status
// {
//     AVAILABLE,
//     BORROWED
// };

// struct Book
// {
//     int id;
//     char title[31];
//     enum Status status;
// };

// int main(int argc, char const *argv[])
// {
//     /*
//     定义一个 Status 枚举类型，包含书籍状态：AVAILABLE 和 BORROWED。
//     定义一个 Book 结构体，包含以下字段：
//     o 书籍ID id；
//     o 书名 title（字符串，长度不超过30）；
//     o 状态 Status。
//     在主函数中：
//     o 创建三个 Book 实例；
//     o 初始化信息并输出每本书的状态。
//     提示：使用 printf 格式化输出书籍信息。
//     */
//     struct Book book1 = {1, "书本1", AVAILABLE};
//     struct Book book2 = {2, "书本2", BORROWED};
//     struct Book book3 = {3, "书本3", AVAILABLE};

//     void printBook(struct Book book);
//     printBook(book1);
//     printBook(book2);
//     printBook(book3);

//     return 0;
// }

// void printBook(struct Book book)
// {
//     char statusStr[10];
//     switch (book.status)
//     {
//     case AVAILABLE:
//         strcpy(statusStr, "可用");
//         break;
//     case BORROWED:
//         strcpy(statusStr, "已借出");
//         break;
//     default:
//         break;
//     }
//     printf("书籍ID为：%d 标题为：%s 的书籍状态是：%s\n", book.id, book.title, statusStr);
// }

// #include <stdio.h>
// #include <string.h>

// enum SensorType
// {
//     TEMPERATURE,
//     HUMIDITY,
//     PRESSURE
// };
// union SensorData
// {
//     float temperature;
//     int humidity;
//     float pressure;
// };

// int main(int argc, char const *argv[])
// {
//     /*
//     定义一个 SensorType 枚举类型：TEMPERATURE、HUMIDITY、PRESSURE。
//     定义一个联合体 SensorData，包含：
//     o temperature（float）；
//     o humidity（int，0-100）；
//     o pressure（float）。
//     在主函数中：
//     o 创建一个变量 sensor_type，表示当前传感器类型；
//     o 创建一个 SensorData 变量，根据传感器类型输入数据；
//     o 输出数据到控制台。
//     提示：使用 switch 根据传感器类型输入和输出相应数据。
//      */
//     enum SensorType sensor_type;
//     union SensorData sensorData;

//     int choice;

//     printf("请选择传感器类型：\n");
//     printf("0 - 温度传感器\n");
//     printf("1 - 湿度传感器\n");
//     printf("2 - 压力传感器\n");
//     printf("请输入选择：");
//     scanf("%d", &choice);

//     sensor_type = choice;

//     switch (sensor_type)
//     {
//     case TEMPERATURE:
//         printf("请输入温度值：");
//         scanf("%f", &sensorData.temperature);

//         printf("当前传感器类型：温度传感器\n");
//         printf("温度数据：%.2f ℃\n", sensorData.temperature);
//         break;

//     case HUMIDITY:
//         printf("请输入湿度值 0-100：");
//         scanf("%d", &sensorData.humidity);

//         if (sensorData.humidity < 0 || sensorData.humidity > 100)
//         {
//             printf("湿度输入错误，湿度范围应为 0-100\n");
//         }
//         else
//         {
//             printf("当前传感器类型：湿度传感器\n");
//             printf("湿度数据：%d %%\n", sensorData.humidity);
//         }
//         break;

//     case PRESSURE:
//         printf("请输入压力值：");
//         scanf("%f", &sensorData.pressure);

//         printf("当前传感器类型：压力传感器\n");
//         printf("压力数据：%.2f Pa\n", sensorData.pressure);
//         break;

//     default:
//         printf("传感器类型选择错误！\n");
//         break;
//     }

//     return 0;
// }

// #include <stdio.h>
// #include <string.h>
// enum LightState
// {
//     RED,
//     YELLOW,
//     GREEN
// };

// int main(int argc, char const *argv[])
// {
//     /*
//     定义一个 LightState 枚举类型，表示交通灯的颜色：RED、YELLOW 和 GREEN。
//     在主函数中：
//     o 定义一个变量 current_light，类型为 LightState；
//     o 使用 switch 语句模拟交通灯颜色变化逻辑（红->绿->黄->红）；
//     o 输出当前交通灯的颜色，并循环切换交通灯颜色。
//     提示：每次输出后切换交通灯颜色，可以使用简单的 for 循环来模拟变化多次。
//     */
//     enum LightState current_light = RED;

//     for (int i = 0; i < 10; i++)
//     {
//         switch (current_light)
//         {
//         case RED:
//             printf("当前红绿灯的颜色为红色\n");
//             current_light = GREEN;
//             break;
//         case GREEN:
//             printf("当前红绿灯的颜色为绿色\n");
//             current_light = YELLOW;
//             break;
//         case YELLOW:
//             printf("当前红绿灯的颜色为黄色\n");
//             current_light = RED;
//             break;
//         default:
//             printf("颜色不支持\n");
//             break;
//         }
//     }

//     return 0;
// }

// #include <stdio.h>
// #include <string.h>

// enum Gender
// {
//     MALE,
//     FEMALE
// };
// struct Student
// {
//     int id;
//     char name[21];
//     enum Gender gender;
//     double score;
// };

// int main(int argc, char const *argv[])
// {

//     /*

//     定义一个 Gender 枚举类型：MALE 和 FEMALE。
//     定义一个 Student 结构体，包含以下字段：
//     o 学号 id；
//     o 姓名 name（字符串，长度不超过20）；
//     o 性别 Gender；
//     o 成绩 score。
//     在主函数中：
//     o 创建三个学生的实例；
//     o 初始化并打印每个学生的信息。
//     提示：使用 printf 格式化输出学生的信息。
//     */
//     struct Student student1 = {1, "学生1", MALE, 96.5};
//     struct Student student2 = {2, "学生2", FEMALE, 86.5};
//     struct Student student3 = {3, "学生3", MALE, 76.5};

//     void printStu(struct Student student1);
//     printStu(student1);
//     printStu(student2);
//     printStu(student3);

//     return 0;
// }
// void printStu(struct Student stu)
// {
//     char genStr[10];
//     if (stu.gender == MALE)
//     {
//         strcpy(genStr, "男");
//     }
//     else
//     {
//         strcpy(genStr, "女");
//     }
//     printf("学生的学号是：%d，名字是：%s，性别是：%s，分数是：%.1lf\n", stu.id, stu.name, genStr, stu.score);
// }

/*
请编写一个程序，定义一个结构体Person, 包含name age，通过scanf printf输入输出该结构体成员
*/
// struct Person
// {
//     char name[10];
//     int age;
// } person;
// printf("请输入姓名\n");
// scanf("%s", &person.name);
// printf("请输入年龄\n");
// scanf("%d", &person.age);
// printf("该结构体name = %s，age = %d\n", person.name, person.age);