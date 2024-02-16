#ifndef DELEGATE_H
#define DELEGATE_H

// ��������Ʈ Ŭ����
template <typename T>
class Delegate {
public:
    // ��� �Լ� ������ Ÿ�� ����
    using MemberFunctionPointer = bool (T::*)(int);

    //// ��������Ʈ ������
    Delegate()
        : object(nullptr), memberFunction(nullptr) {}

    // ��������Ʈ�� �Լ� ����
    void Connect(T* obj, MemberFunctionPointer func) {
        object = obj;
        memberFunction = func;
    }

    // ��������Ʈ���� �Լ� ����
    void Disconnect() {
        object = nullptr;
        memberFunction = nullptr;
    }

    // ��������Ʈ ȣ�� �Լ�
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