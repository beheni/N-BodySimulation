#include "BitonicSort.h"

BitonicSort::BitonicSort(const std::string& shaderFile)
{
    m_ComputeProgram = std::make_unique<ComputeProgram>(shaderFile);

    // query the hardware limits
    GLint workGroupSizeHardwareLimit = 0;
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSizeHardwareLimit);
    while (m_MaxComputeWorkGroupSize * 2 < workGroupSizeHardwareLimit)
        m_MaxComputeWorkGroupSize *= 2;
  
}

void BitonicSort::Sort(int n)
{
    uint32_t workgroup_size_x = 0;
    if (n < m_MaxComputeWorkGroupSize * 2) 
    {
        workgroup_size_x = n / 2;
    }
    else 
    {
        workgroup_size_x = m_MaxComputeWorkGroupSize;
    }

    m_WorkGroupCount = n / (workgroup_size_x * 2);

    uint32_t h = workgroup_size_x * 2;

    m_ComputeProgram->Use();

    DispatchLocalBMS(n, h);

    // we must now double h, as this happens before every flip
    h *= 2;

    for (; h <= n; h *= 2) {

        DispatchBigFlip(n, h);

        for (uint32_t hh = h / 2; hh > 1; hh /= 2) {

            if (hh <= workgroup_size_x * 2) {
                DispatchLocalDisperse(n, hh);
                break;
            }
            else {
                DispatchBigDisperse(n, hh);
            }
        }
    }
}

void BitonicSort::DispatchLocalBMS(uint32_t n, uint32_t h)
{
    m_ComputeProgram->SetInt("u_Algorithm", LocalBMS);
    Dispatch(n, h);
}

void BitonicSort::DispatchBigFlip(uint32_t n, uint32_t h)
{
    m_ComputeProgram->SetInt("u_Algorithm", BigFlip);
    Dispatch(n, h);
}

void BitonicSort::DispatchLocalDisperse(uint32_t n, uint32_t h)
{
    m_ComputeProgram->SetInt("u_Algorithm", LocalDisperse);
    Dispatch(n, h);
}

void BitonicSort::DispatchBigDisperse(uint32_t n, uint32_t h)
{
    m_ComputeProgram->SetInt("u_Algorithm", BigDisperse);
    Dispatch(n, h);
}

void BitonicSort::Dispatch(uint32_t n, uint32_t h)
{
    m_ComputeProgram->SetInt("u_N", n);
    m_ComputeProgram->SetInt("u_H", h);
    glDispatchCompute(m_WorkGroupCount, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    // TODO: consider other barriers
}
