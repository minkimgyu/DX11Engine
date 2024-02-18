#ifndef DELEGATE_H
#define DELEGATE_H

// ��������Ʈ Ŭ����
template <typename T1, typename T2, typename... T3>
class Delegate {
public:
    // ��� �Լ� ������ Ÿ�� ����
    using MemberFunctionPointer = T1(T2::*)(T3...);

    //// ��������Ʈ ������
    Delegate()
        : object(nullptr), memberFunction(nullptr) {}

    // ��������Ʈ�� �Լ� ����
    void Connect(T2* obj, MemberFunctionPointer func) {
        object = obj;
        memberFunction = func;
    }

    // ��������Ʈ���� �Լ� ����
    void Disconnect() {
        object = nullptr;
        memberFunction = nullptr;
    }

    // ��������Ʈ ȣ�� �Լ�
    T1 Invoke(T3... value) {
        return (object->*memberFunction)(value...);
    }

private:
    T2* object;
    MemberFunctionPointer memberFunction;
};

#endif // DELEGATE_H