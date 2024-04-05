#pragma once

#include <device/gekko/execution.h>

class X64JitEngine : public IExecutionEngine
{
public:
    explicit X64JitEngine(class GekkoCpu& cpu) : mCpu{ &cpu } {}
    ~X64JitEngine() override = default;


    size_t Execute(addr_t startingPc, size_t targetInstrCount) override;

private:
    GekkoCpu* mCpu;
};
