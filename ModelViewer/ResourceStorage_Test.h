#pragma once

#include <cassert>
#include <iostream>

#include "ResourceStorage.h"

void resourceStorageDefaultTest() {
    ResourceStorage<int> S;

    S.Insert("a", 1);
    S.Insert("b", 2);
    S.Insert("c", 3);

    // 정상동작
    assert(1 == S.Get("a"));

    // 존재하지 않는 인덱스 접근시
    try {
        S.Get("d");
        assert(false);
    } catch (std::exception &e) {
        std::cout << "정상적으로 예외가 출력됨 =" << e.what() << std::endl;
    }

    // 중복 삽입시 업데이트
    S.Insert("a", 111);
    assert(S.Get("a") == 111);

    // 제거 확인
    S.Delete("a");
    try {
        S.Get("a");
        assert(false);
    } catch (std::exception &e) {
        std::cout << "정상적으로 예외가 출력됨 =" << e.what() << std::endl;
    }
}