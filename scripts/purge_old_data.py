import asyncio
import asyncpg
import os

async def purge_old():
    db_url = os.getenv("DATABASE_URL")
    if not db_url:
        raise RuntimeError("DATABASE_URL is not set")
    
    conn = await asyncpg.connect(db_url)
    result = await conn.execute("""
        DELETE FROM transactions
        WHERE timestamp < NOW() - INTERVAL '7 days'
    """)
    await conn.close()
    print(f"[INFO] Purge completed: {result}")

if __name__ == "__main__":
    asyncio.run(purge_old())
