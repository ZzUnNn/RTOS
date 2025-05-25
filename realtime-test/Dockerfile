FROM arm32v7/python:3.9-slim
WORKDIR /app
COPY app/requirements.txt .
COPY app/realtime_monitor.py .
RUN apt-get update && apt-get install -y \
    python3-dev \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

RUN pip install --no-cache-dir -r requirements.txt
CMD ["python3", "realtime_monitor.py"]
