#pragma once
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include "class.hpp"

static uintptr_t ubase;
static uintptr_t gbase;
static uintptr_t exebase;
static uintptr_t gameobjectmanager_offsets;
static uintptr_t basenetworkable_offsets;


namespace th = std::this_thread;
namespace ch = std::chrono;

namespace rfo
{
	std::unique_ptr<memory_rfo> memory = nullptr;
}

