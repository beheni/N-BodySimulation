#include "BitonicSort.h"

BitonicSort::BitonicSort(const std::string& shaderFile, size_t numberOfElements)
    :
    m_NumberOfElemetes(numberOfElements)
{
    m_ComputeProgram = std::make_unique<ComputeProgram>(shaderFile);

    // query the hardware limits
    GLint workGroupSizeHardwareLimit{};
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSizeHardwareLimit);
    //while (m_MaxComputeWorkGroupSize * 2 < workGroupSizeHardwareLimit)
    //    m_MaxComputeWorkGroupSize *= 2;

    m_WorkGroupCount = m_NumberOfElemetes / 1024;
}

void BitonicSort::Use()
{
    m_ComputeProgram->Use();
}

void BitonicSort::Sort()
{
    for (size_t h = 2; h <= m_NumberOfElemetes; h *= 2)
    {
        DoFlip(h);
        for (size_t hh = h / 2; hh > 1; hh /= 2)
        {
            DoDisperse(hh);
        }
    }
}

void BitonicSort::DoFlip(uint32_t h)
{
    m_ComputeProgram->SetInt("u_Algorithm", Algorithm::Flip);
    Dispatch(h);
}

void BitonicSort::DoDisperse(uint32_t h)
{
    m_ComputeProgram->SetInt("u_Algorithm", Algorithm::Disperse);
    Dispatch(h);
}

void BitonicSort::Dispatch(uint32_t h)
{
    m_ComputeProgram->SetInt("u_H", h);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
    glDispatchCompute(m_WorkGroupCount, 1, 1);
    // TODO: consider other barriers
}
