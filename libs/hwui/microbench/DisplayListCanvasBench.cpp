/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <benchmark/Benchmark.h>
#include <utils/Singleton.h>

#include "DisplayList.h"
#include "DisplayListCanvas.h"
#include "microbench/MicroBench.h"

using namespace android;
using namespace android::uirenderer;

BENCHMARK_NO_ARG(BM_DisplayListData_alloc);
void BM_DisplayListData_alloc::Run(int iters) {
    StartBenchmarkTiming();
    for (int i = 0; i < iters; ++i) {
        auto data = new DisplayListData();
        MicroBench::DoNotOptimize(data);
        delete data;
    }
    StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_DisplayListData_alloc_theoretical);
void BM_DisplayListData_alloc_theoretical::Run(int iters) {
    StartBenchmarkTiming();
    for (int i = 0; i < iters; ++i) {
        auto data = new char[sizeof(DisplayListData)];
        MicroBench::DoNotOptimize(data);
        delete[] data;
    }
    StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_DisplayListCanvas_record_empty);
void BM_DisplayListCanvas_record_empty::Run(int iters) {
    DisplayListCanvas canvas(100, 100);
    canvas.finishRecording();

    StartBenchmarkTiming();
    for (int i = 0; i < iters; ++i) {
        canvas.reset(100, 100);
        MicroBench::DoNotOptimize(&canvas);
        canvas.finishRecording();
    }
    StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_DisplayListCanvas_record_saverestore);
void BM_DisplayListCanvas_record_saverestore::Run(int iters) {
    DisplayListCanvas canvas(100, 100);
    canvas.finishRecording();

    StartBenchmarkTiming();
    for (int i = 0; i < iters; ++i) {
        canvas.reset(100, 100);
        canvas.save(SkCanvas::kMatrixClip_SaveFlag);
        canvas.save(SkCanvas::kMatrixClip_SaveFlag);
        MicroBench::DoNotOptimize(&canvas);
        canvas.restore();
        canvas.restore();
        canvas.finishRecording();
    }
    StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_DisplayListCanvas_record_translate);
void BM_DisplayListCanvas_record_translate::Run(int iters) {
    DisplayListCanvas canvas(100, 100);
    canvas.finishRecording();

    StartBenchmarkTiming();
    for (int i = 0; i < iters; ++i) {
        canvas.reset(100, 100);
        canvas.scale(10, 10);
        MicroBench::DoNotOptimize(&canvas);
        canvas.finishRecording();
    }
    StopBenchmarkTiming();
}

class NullClient: public CanvasStateClient {
    void onViewportInitialized() override {}
    void onSnapshotRestored(const Snapshot& removed, const Snapshot& restored) {}
    GLuint getTargetFbo() const override { return 0; }
};

BENCHMARK_NO_ARG(BM_CanvasState_saverestore);
void BM_CanvasState_saverestore::Run(int iters) {
    NullClient client;
    CanvasState state(client);
    state.initializeSaveStack(100, 100, 0, 0, 100, 100, Vector3());

    StartBenchmarkTiming();
    for (int i = 0; i < iters; ++i) {
        state.save(SkCanvas::kMatrixClip_SaveFlag);
        state.save(SkCanvas::kMatrixClip_SaveFlag);
        MicroBench::DoNotOptimize(&state);
        state.restore();
        state.restore();
    }
    StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_CanvasState_init);
void BM_CanvasState_init::Run(int iters) {
    NullClient client;
    CanvasState state(client);
    state.initializeSaveStack(100, 100, 0, 0, 100, 100, Vector3());

    StartBenchmarkTiming();
    for (int i = 0; i < iters; ++i) {
        state.initializeSaveStack(100, 100, 0, 0, 100, 100, Vector3());
        MicroBench::DoNotOptimize(&state);
    }
    StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_CanvasState_translate);
void BM_CanvasState_translate::Run(int iters) {
    NullClient client;
    CanvasState state(client);
    state.initializeSaveStack(100, 100, 0, 0, 100, 100, Vector3());

    StartBenchmarkTiming();
    for (int i = 0; i < iters; ++i) {
        state.translate(5, 5, 0);
        MicroBench::DoNotOptimize(&state);
        state.translate(-5, -5, 0);
    }
    StopBenchmarkTiming();
}
