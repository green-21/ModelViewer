#pragma once

#include <cassert>
#include <iostream>

#include "ResourceStorage.h"

void resourceStorageDefaultTest() {
    ResourceStorage<int> S;

    S.Insert("a", 1);
    S.Insert("b", 2);
    S.Insert("c", 3);

    // ������
    assert(1 == S.Get("a"));

    // �������� �ʴ� �ε��� ���ٽ�
    try {
        S.Get("d");
        assert(false);
    } catch (std::exception &e) {
        std::cout << "���������� ���ܰ� ��µ� =" << e.what() << std::endl;
    }

    // �ߺ� ���Խ� ������Ʈ
    S.Insert("a", 111);
    assert(S.Get("a") == 111);

    // ���� Ȯ��
    S.Delete("a");
    try {
        S.Get("a");
        assert(false);
    } catch (std::exception &e) {
        std::cout << "���������� ���ܰ� ��µ� =" << e.what() << std::endl;
    }
}