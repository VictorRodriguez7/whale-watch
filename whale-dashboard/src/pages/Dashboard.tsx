import { useEffect, useState } from "react";

interface Transaction {
  id: number;
  tx_hash: string;
  from_address: string;
  to_address: string;
  value_eth: number;
  block_number: number;
  timestamp: string;
}

interface Stats {
  latest_block: number;
  total_whales: number;
  avg_value_eth: number;
}

export default function Dashboard() {
  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [stats, setStats] = useState<Stats | null>(null);
  const [status, setStatus] = useState("Connecting...");

  useEffect(() => {
    let ws: WebSocket;
    let reconnectTimeout: ReturnType<typeof setTimeout>;

    const connect = () => {
      setStatus("Connecting...");
      ws = new WebSocket("ws://localhost:8000/ws");

      ws.onopen = () => {
        console.log("✅ WebSocket connected");
        setStatus("Connected");
      };

      ws.onmessage = (event) => {
        try {
          const data = JSON.parse(event.data);
          setStats(data.stats);
          setTransactions(data.transactions);
        } catch (err) {
          console.error("Error parsing message:", err);
        }
      };

      ws.onerror = () => {
        console.error("WebSocket error");
      };

      ws.onclose = () => {
        console.warn("WebSocket closed. Reconnecting...");
        setStatus("Reconnecting...");
        reconnectTimeout = setTimeout(connect, 3000);
      };
    };

    connect();

    return () => {
      if (ws) ws.close();
      if (reconnectTimeout) clearTimeout(reconnectTimeout);
    };
  }, []);

  return (
    <div className="p-6">
      <h1 className="text-2xl font-bold mb-4">Whale Dashboard</h1>
      <p className="mb-4 text-sm text-gray-500">Status: {status}</p>

      {stats && (
        <div className="grid grid-cols-3 gap-4 mb-4">
          <div className="p-4 bg-gray-100 rounded">Latest Block: {stats.latest_block}</div>
          <div className="p-4 bg-gray-100 rounded">Total Whales: {stats.total_whales}</div>
          <div className="p-4 bg-gray-100 rounded">Avg ETH: {stats.avg_value_eth.toFixed(2)}</div>
        </div>
      )}

      <table className="table-auto w-full border">
        <thead>
          <tr className="bg-gray-200">
            <th>Tx Hash</th>
            <th>From</th>
            <th>To</th>
            <th>Value ETH</th>
            <th>Block</th>
            <th>Time</th>
          </tr>
        </thead>
        <tbody>
          {transactions.map((tx) => (
            <tr key={tx.id} className="border-b">
              <td>{tx.tx_hash.slice(0, 10)}...</td>
              <td>{tx.from_address.slice(0, 10)}...</td>
              <td>{tx.to_address.slice(0, 10)}...</td>
              <td>{tx.value_eth.toFixed(2)}</td>
              <td>{tx.block_number}</td>
              <td>{new Date(tx.timestamp).toLocaleTimeString()}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
