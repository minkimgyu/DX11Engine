#ifndef DELEGATE_H
#define DELEGATE_H

// 델리게이트 클래스
template <typename T1, typename T2, typename... T3>
class Delegate {
public:
    // 멤버 함수 포인터 타입 정의
    using MemberFunctionPointer = T1(T2::*)(T3...);

    //// 델리게이트 생성자
    Delegate()
        : object(nullptr), memberFunction(nullptr) {}

    // 델리게이트에 함수 연결
    void Connect(T2* obj, MemberFunctionPointer func) {
        object = obj;
        memberFunction = func;
    }

    // 델리게이트에서 함수 해제
    void Disconnect() {
        object = nullptr;
        memberFunction = nullptr;
    }

    // 델리게이트 호출 함수
    T1 Invoke(T3... value) {
        return (object->*memberFunction)(value...);
    }

private:
    T2* object;
    MemberFunctionPointer memberFunction;
};

#endif // DELEGATE_H