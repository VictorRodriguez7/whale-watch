from services.db import get_db

async def get_transactions(limit: int, offset: int, min_eth: float = None, address: str = None):
    conn = await get_db()
    query = """
        SELECT id, tx_hash, block_number, from_address, to_address, value_eth, timestamp
        FROM transactions
        WHERE 1=1
    """
    params = []

    if min_eth is not None:
        query += f" AND value_eth >= ${len(params)+1}"
        params.append(min_eth)

    if address:
        query += f" AND (from_address = ${len(params)+1} OR to_address = ${len(params)+1})"
        params.append(address)

    query += f" ORDER BY block_number DESC LIMIT ${len(params)+1} OFFSET ${len(params)+2}"
    params.extend([limit, offset])

    rows = await conn.fetch(query, *params)
    await conn.close()

    return {"transactions": [dict(row) for row in rows]}

async def get_stats():
    conn = await get_db()
    latest_block = await conn.fetchval("SELECT MAX(block_number) FROM transactions")
    whale_count = await conn.fetchval("SELECT COUNT(*) FROM transactions")
    avg_value_eth = await conn.fetchval("SELECT AVG(value_eth) FROM transactions")
    await conn.close()
    return {
        "latest_block": latest_block,
        "total_whales": whale_count,
        "avg_value_eth": float(avg_value_eth) if avg_value_eth else 0
    }
