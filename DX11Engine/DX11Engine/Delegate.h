#ifndef DELEGATE_H
#define DELEGATE_H

// 델리게이트 클래스
template <typename T>
class Delegate {
public:
    // 멤버 함수 포인터 타입 정의
    using MemberFunctionPointer = bool (T::*)(int);

    //// 델리게이트 생성자
    Delegate()
        : object(nullptr), memberFunction(nullptr) {}

    // 델리게이트에 함수 연결
    void Connect(T* obj, MemberFunctionPointer func) {
        object = obj;
        memberFunction = func;
    }

    // 델리게이트에서 함수 해제
    void Disconnect() {
        object = nullptr;
        memberFunction = nullptr;
    }

    // 델리게이트 호출 함수
    bool Invoke(int value) {
        if (object && memberFunction) {
            return (object->*memberFunction)(value);
        }
        return false;
    }

private:
    T* object;
    MemberFunctionPointer memberFunction;
};

#endif // DELEGATE_H