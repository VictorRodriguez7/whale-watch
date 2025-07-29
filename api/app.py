from fastapi import FastAPI, WebSocket
from fastapi.middleware.cors import CORSMiddleware
import json
import asyncio
from services.queries import get_transactions, get_stats
from decimal import Decimal
from datetime import datetime

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # In production, replace with React domain
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class CustomEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, Decimal):
            return float(obj)
        if isinstance(obj, datetime):
            return obj.isoformat()  
        return super().default(obj)

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    print("WebSocket client connected")

    try:
        while True:
            try:
                stats = await get_stats()
                transactions = await get_transactions(limit=10, offset=0)

                data = {
                    "stats": stats,
                    "transactions": transactions["transactions"]
                }

                print(f"Sending data: {data}")
                await websocket.send_text(json.dumps(data, cls=CustomEncoder))
            except Exception as e:
                print(f"Error inside loop: {e}")

            await asyncio.sleep(5)

    except Exception as e:
        print(f"WebSocket connection closed: {e}")

@app.get("/transactions")
async def transactions(limit: int = 100, offset: int = 0, min_eth: float = None, address: str = None):
    return await get_transactions(limit, offset, min_eth, address)

@app.get("/stats")
async def stats():
    return await get_stats()
