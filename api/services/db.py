import os
import asyncpg

DATABASE_URL = os.getenv("DATABASE_URL")

async def get_db():
    if not DATABASE_URL:
        raise Exception("DATABASE_URL not set")
    return await asyncpg.connect(DATABASE_URL)
