start TestRunControl.exe
timeout /T 1  > nul
start TestLogCollector.exe
timeout /T 1  > nul
start TestDataCollector.exe
timeout /T 1  > nul

timeout /T 1  > nul
start TLUProducer.exe